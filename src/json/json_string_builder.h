#pragma once

#include <cstddef>
#include <cstring>
#include <string>

class JsonStringBuilder
{
    char* m_ptr;
    size_t pos = 0;

public:
    JsonStringBuilder(char* ptr) : m_ptr(ptr)
    {}

    inline void write_char(char c)
    {
        m_ptr[pos++] = c;
    }

    inline void write_raw(const char* data, size_t len)
    {
        memcpy(m_ptr + pos, data, len);
        pos += len;
    }

    inline std::string finish()
    {
        return std::string(m_ptr, pos);
    }
};