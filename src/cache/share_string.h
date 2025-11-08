#pragma once

#include <string>
#include <atomic>
#include <fmt/core.h>
#include <fmt/format.h>

#include <cache/cache_pool.h>

#define MAX_STRING_NUM 1000000

struct StringReference
{
    std::string data;
    std::atomic<uint32_t> count;

    inline void retain()
    {
        count.fetch_add(1, std::memory_order_relaxed);
    }

    inline uint32_t release()
    {
        return count.fetch_sub(1, std::memory_order_acq_rel);
    }

    static std::string name()
    {
        return "StringReference";
    }
};

using StringPool = CachePool<StringReference, MAX_STRING_NUM>;

class ShareString
{
    StringReference* m_string_reference = nullptr;
    size_t m_start_index = 0;
    size_t m_length = 0;

public:
    ShareString();
    ShareString(const std::string& data);
    ShareString(std::string&& data);

    ShareString(const ShareString&);
    ShareString(ShareString&&);
    ShareString& operator=(const ShareString&);
    ShareString& operator=(ShareString&&);

    ShareString from_substr(size_t start_index, size_t length) const;
    ShareString from_substr_with_end(size_t start_index, size_t end_index) const;

    ~ShareString();

    inline std::string_view data() const
    {
        if (!m_string_reference) return {};
        return std::string_view(m_string_reference->data).substr(m_start_index, m_length);
    }

    inline std::string to_string() const
    {
        return std::string(data());
    }

    inline operator std::string() const
    {
        return std::string(data());
    }

    inline bool operator==(const ShareString& other) const
    {
        if (!m_string_reference || !other.m_string_reference)
        {
            return false; // If either is null, they are not equal
        }
        return data() == other.data();
    }

private:
    inline std::string_view substr(size_t start_index, size_t length) const
    {
        if (!m_string_reference) return {};

        if (m_start_index + start_index + length <= m_length)
        {
            return std::string_view(m_string_reference->data).substr(m_start_index + start_index, length);
        }
        else
        {
            throw std::out_of_range("ShareString - Substr out of range, start_index: " + std::to_string(start_index) + ", length: " + std::to_string(length) + ", m_length: " + std::to_string(m_length));
        }
    }

    inline void check_release_current_data();
};

template <>
struct fmt::formatter<ShareString> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const ShareString& data, FormatContext& ctx)
    {
        return fmt::formatter<std::string_view>::format(data.data(), ctx);
    }
};