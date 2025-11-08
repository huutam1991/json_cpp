#pragma once

#include <string>
#include <memory>
#include <functional>

#include <json/json_string_builder.h>

class JsonTypeBase
{
protected:
    JsonTypeBase(const JsonTypeBase&) = delete;
    JsonTypeBase() = default;

public:
    virtual ~JsonTypeBase() {};

protected:
    using IsJsonValuePtr = bool (JsonTypeBase::*)();
    IsJsonValuePtr is_json_value_ptr = nullptr;

    using GetCopyPtr = JsonTypeBase* (JsonTypeBase::*)();
    GetCopyPtr get_copy_ptr = nullptr;
    GetCopyPtr get_deep_clone_ptr = nullptr;

    using WriteStringValuePtr = void (JsonTypeBase::*)(JsonStringBuilder& builder);
    WriteStringValuePtr write_string_value_ptr = nullptr;

    using ReleasePtr = void (JsonTypeBase::*)();
    ReleasePtr release_ptr = nullptr;

public:
    inline bool is_json_value()
    {
        if (is_json_value_ptr != nullptr)
        {
            return (this->*is_json_value_ptr)();
        }
        return false;
    }

    inline JsonTypeBase* get_copy()
    {
        if (get_copy_ptr != nullptr)
        {
            return (this->*get_copy_ptr)();
        }
        return nullptr;
    }

    inline JsonTypeBase* get_deep_clone()
    {
        if (get_deep_clone_ptr != nullptr)
        {
            return (this->*get_deep_clone_ptr)();
        }
        return nullptr;
    }

    inline void write_string_value(JsonStringBuilder& builder)
    {
        if (write_string_value_ptr != nullptr)
        {
            (this->*write_string_value_ptr)(builder);
        }
    }

    inline void release()
    {
        if (release_ptr != nullptr)
        {
            (this->*release_ptr)();
        }
    }
};
