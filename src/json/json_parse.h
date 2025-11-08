#pragma once

#include <json/json.h>
#include <cache/share_string.h>

#define TRUE_VALUE 1
#define FALSE_VALUE 2
#define NOT_BOOLEAN_VALUE 0

class JsonParseNew
{
    ShareString m_share_string;
    std::string_view m_object_string;
    size_t m_size;
    bool m_is_valid = true;

    bool check_exceed_size(size_t index);
    ShareString get_sub_string(size_t start, size_t end);

    bool is_number(char c)
    {
        int n = c - '0';
        return (n >= 0 && n <= 9) || c == '-';
    }

    bool is_letter(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    int is_boolean(const char* m_object_string, size_t start, size_t size)
    {
        // Check true value
        if (start + 4 < size &&
            is_letter(m_object_string[start + 4]) == false &&
            m_object_string[start]     == 't' &&
            m_object_string[start + 1] == 'r' &&
            m_object_string[start + 2] == 'u' &&
            m_object_string[start + 3] == 'e')
        {
            return TRUE_VALUE;
        }
        else if (start + 5 < size &&
            is_letter(m_object_string[start + 5]) == false &&
            m_object_string[start]     == 'f' &&
            m_object_string[start + 1] == 'a' &&
            m_object_string[start + 2] == 'l' &&
            m_object_string[start + 3] == 's' &&
            m_object_string[start + 4] == 'e'
        )
        {
            return FALSE_VALUE;
        }

        return NOT_BOOLEAN_VALUE;
    }

    bool is_null(const char* m_object_string, size_t start, size_t size)
    {
        // Check true value
        if (start + 4 < size &&
            is_letter(m_object_string[start + 4]) == false &&
            is_number(m_object_string[start + 4]) == false &&
            m_object_string[start]     == 'n' &&
            m_object_string[start + 1] == 'u' &&
            m_object_string[start + 2] == 'l' &&
            m_object_string[start + 3] == 'l')
        {
            return true;
        }

        return false;
    }

public:
    JsonParseNew(std::string object);
    Json parse();

private:
    Json parse_object(size_t& start_pos);
    Json parse_array(size_t& start_pos);
    Json parse_value_number(size_t& start_pos);
    Json parse_value(size_t& start_pos);
    std::string parse_key(size_t& start_pos);
    ShareString parse_value_string(size_t& start_pos);
};