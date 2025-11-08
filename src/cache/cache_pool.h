#pragma once

#include <cxxabi.h>
#include <cstddef>
#include <string>
#include <array>
#include <atomic>

#define FORCE_INLINE inline __attribute__((always_inline))

template <typename T>
constexpr std::string demangled_name()
{
    int status;
    char* realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::string result = (status == 0 && realname) ? realname : typeid(T).name();
    std::free(realname);
    return result;
}

template <typename T, typename U = void>
struct TypeName
{
    static constexpr std::string name()
    {
        return demangled_name<T>();
    }
};

template <typename T>
struct TypeName<T, std::void_t<decltype(T::name())>>
{
    static constexpr std::string name()
    {
        return T::name();
    }
};

template <typename T>
concept has_init = requires
{
    { &T::init };
};

template <typename T>
concept has_clear = requires
{
    { &T::clear };
};

template <class T, size_t Size>
class CachePool
{
    struct alignas(64) ObjectWrapper
    {
        T object;
    };

    struct alignas(64) ObjectPointerWrapper
    {
        T* ptr;
    };

    struct PoolBuffer
    {
        alignas(64) std::array<ObjectPointerWrapper, Size> available_items;
        alignas(64) std::array<ObjectWrapper, Size> data;
        alignas(64) std::atomic<size_t> head = 0;
        alignas(64) std::atomic<size_t> tail = 0;
        alignas(64) std::atomic<size_t> size = Size;

        PoolBuffer()
        {
            for (size_t i = 0; i < Size; ++i)
            {
                available_items[i].ptr = &data[i].object;
            }
        }

        FORCE_INLINE size_t get_current_head()
        {
            size_t current = head.load(std::memory_order_acquire);
            size_t next = (current + 1) % Size;

            while (!head.compare_exchange_weak(current, next,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed))
            {
                next = (current + 1) % Size;
            }

            return current;
        }

        FORCE_INLINE size_t get_current_tail()
        {
            size_t current = tail.load(std::memory_order_acquire);
            size_t next = (current + 1) % Size;

            while (!tail.compare_exchange_weak(current, next,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed))
            {
                next = (current + 1) % Size;
            }

            return current;
        }
    };

    FORCE_INLINE static PoolBuffer& get_pool_buffer()
    {
        static PoolBuffer* pool_buffer = new PoolBuffer();
        return *pool_buffer;
    }

public:
    // Acquire a cache item
    FORCE_INLINE static T* acquire()
    {
        static std::string name = TypeName<T>::name();

        T* item;
        {
            // MeasureTime measure_time("CachePool::acquire, name: " + name, MeasureUnit::NANOSECOND);
            // MeasureTime measure_time("CachePool::acquire", MeasureUnit::NANOSECOND);

            PoolBuffer& pool_buffer = get_pool_buffer();
            if (pool_buffer.size.load(std::memory_order_relaxed) == 0)
            {
                throw std::runtime_error("No available items in cache pool: [" + TypeName<T>::name() + "]");
            }

            // Get the item from the pool
            size_t head_index = pool_buffer.get_current_head();
            item = pool_buffer.available_items[head_index].ptr;

            // Decrease size only after successfully moving head
            pool_buffer.size.fetch_sub(1, std::memory_order_release);
        }

        // Check if the item has init method and call it
        if constexpr (has_init<T>)
        {
            item->init();
        }

        return item;
    }

    // Release a cache item back to the pool
    FORCE_INLINE static void release(T* item)
    {
        static std::string name = TypeName<T>::name();

        if (item != nullptr)
        {
            {
                // MeasureTime measure_time("CachePool::release, name: " + name, MeasureUnit::NANOSECOND);
                // MeasureTime measure_time("CachePool::release", MeasureUnit::NANOSECOND);

                // Add item back to the pool
                PoolBuffer& pool_buffer = get_pool_buffer();
                size_t tail_index = pool_buffer.get_current_tail();
                pool_buffer.available_items[tail_index].ptr = item;

                // Increase size only after successfully moving tail
                pool_buffer.size.fetch_add(1, std::memory_order_release);
            }

            // Check if the item has clear method and call it
            if constexpr (has_clear<T>)
            {
                item->clear();
            }
        }
        else
        {
            throw std::runtime_error("Attempt to release a null item back to the cache pool: [" + TypeName<T>::name() + "]");
        }
    }

    FORCE_INLINE static size_t head()
    {
        return get_pool_buffer().head.load(std::memory_order_relaxed);
    }

    FORCE_INLINE static size_t tail()
    {
        return get_pool_buffer().tail.load(std::memory_order_relaxed);
    }

    FORCE_INLINE static size_t size()
    {
        return get_pool_buffer().size.load(std::memory_order_relaxed);
    }

    FORCE_INLINE static size_t total_released_items()
    {
        size_t size = get_pool_buffer().size.load(std::memory_order_acquire);
        return Size - size;
    }
};