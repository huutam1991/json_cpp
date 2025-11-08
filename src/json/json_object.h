#pragma once

#include <unordered_map>
#include <vector>
#include <atomic>

#include <json/json_type_base.h>
#include <json/json.h>
#include <cache/cache_pool.h>

class JsonObject;
using JsonObjectPool = CachePool<JsonObject, 1000>;

class JsonObject : public JsonTypeBase
{
    std::atomic<uint32_t> reference_count = 0; // Reference count for shared ownership
    bool m_is_array = false; // Flag to indicate if this is an array
    std::unordered_map<std::string, Json> m_object; // Key-value pairs for JSON object
    std::vector<Json> m_array; // Array for JSON object

public:
    JsonObject();
    JsonObject(const JsonObject&) = delete;
    JsonObject(JsonObject&&) = delete;
    JsonObject& operator=(const JsonObject&) = delete;
    JsonObject& operator=(JsonObject&&) = delete;

    virtual ~JsonObject() override = default;

    void add_pair(std::pair<std::string, Json> field)
    {
        m_is_array = false; // This is an object, not an array
        m_object.insert(std::move(field));
    }

    Json& operator[](const char* key)
    {
        m_is_array = false; // This is an object, not an array
        auto [it, inserted] = m_object.emplace(key, Json());
        return it->second;
    }

    Json& operator[](size_t index)
    {
        m_is_array = true; // This is an array, not an object

        if (index >= m_array.size())
        {
            m_array.resize(index + 1);
        }
        return m_array[index];
    }

    void init()
    {
        reference_count = 1; // Initialize reference count to 1
    }

    void clear()
    {
        m_object.clear();
        m_array.clear();
        m_is_array = false;
    }

    void for_each(std::function<void(Json&)>& loop_func);
    void for_each_with_key(std::function<void(const std::string&,Json&)>& loop_func);
    void for_each_with_index(std::function<void(size_t,Json&)>& loop_func);

    // For iterating through the object
    using iterator = std::unordered_map<std::string, Json>::iterator;
    using const_iterator = std::unordered_map<std::string, Json>::const_iterator;
    iterator begin() { return m_object.begin(); }
    iterator end() { return m_object.end(); }
    const_iterator begin() const { return m_object.begin(); }
    const_iterator end() const { return m_object.end(); }

    bool has_field(const std::string& field) const
    {
        return m_is_array ? false :m_object.find(field) != m_object.end();
    }

    void remove_field(const std::string& field)
    {
        m_is_array = false; // Ensure this is treated as an object
        m_object.erase(field);
    }

    bool is_array() const
    {
        return m_is_array;
    }

    void set_size(size_t size)
    {
        m_is_array = true; // Ensure this is treated as an array
        m_array.resize(size);
    }

    void set_capacity(size_t size)
    {
        m_is_array = true; // Ensure this is treated as an array
        m_array.reserve(size);
    }

    int size() const
    {
        return m_is_array ? m_array.size() : m_object.size();
    }

    int capacity() const
    {
        return m_is_array ? m_array.capacity() : m_object.size();
    }

    void reverse()
    {
        m_is_array = true;
        std::reverse(m_array.begin(), m_array.end());
    }

    void sort(std::function<bool(Json&, Json&)> compare_func)
    {
        m_is_array = true; // Ensure this is treated as an array
        std::sort(m_array.begin(), m_array.end(), compare_func);
    }

    void push_back(const Json& value)
    {
        m_is_array = true; // Ensure this is treated as an array
        m_array.push_back(value);
    }

    // Methods from JsonTypeBase
    inline bool is_json_value()
    {
        return false; // This is not a JSON value, but an object
    }

    inline void write_string_value(JsonStringBuilder& builder)
    {
        if (m_is_array)
        {
            builder.write_char('[');
            for (size_t i = 0; i < m_array.size(); ++i)
            {
                if (i > 0) builder.write_char(',');
                m_array[i].write_string_value(builder);
            }
            builder.write_char(']');
        }
        else
        {
            std::string result = "{";
            builder.write_char('{');
            int count = 0;
            for (const auto& [key, value] : m_object)
            {
                if (count++ > 0) builder.write_char(',');
                builder.write_char('\"');
                builder.write_raw(key.data(), key.size());
                builder.write_char('\"');
                builder.write_char(':');
                value.write_string_value(builder);
            }
            builder.write_char('}');
        }
    }

    inline JsonTypeBase* get_copy()
    {
        reference_count.fetch_add(1, std::memory_order_acq_rel);
        return this;
    }

    inline JsonTypeBase* get_deep_clone()
    {
        JsonObject* clone = JsonObjectPool::acquire();
        clone->m_is_array = m_is_array;

        // Deep clone the array
        clone->m_array.reserve(m_array.size());
        for (auto& item : m_array)
        {
            clone->m_array.push_back(item.deep_clone());
        }

        // Deep clone the object
        for (auto& [key, value] : m_object)
        {
            clone->m_object[key] = value.deep_clone();
        }

        return clone;
    }

    inline void release()
    {
        if (reference_count.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            JsonObjectPool::release(this);
        }
    }
};