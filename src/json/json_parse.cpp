#include <string>
#include <iostream>
#include <charconv>

#include <json/json_parse.h>

ShareString JsonParseNew::get_sub_string(size_t start, size_t end)
{
    size_t size = end - start;
    return ShareString(m_share_string.from_substr(start, size));
}

JsonParseNew::JsonParseNew(std::string object) : m_share_string(std::move(object))
{}

Json JsonParseNew::parse()
{
    Json res;

    m_object_string = m_share_string.data();
    m_size = m_object_string.size();

    if (m_size == 0)
    {
        res = nullptr; // If the object is empty, return null
        return res;
    }

    // Find first character '{'
    size_t start = 0;
    while (m_object_string[start] != '{' && m_object_string[start] != '[')
    {
        start++;
        if (check_exceed_size(start)) break;
    }
    start++;

    res = m_object_string[start - 1] == '{' ? parse_object(start) : parse_array(start);
    if (m_is_valid == false)
    {
        res = nullptr; // If parsing failed, return null
    }

    return res;
}

bool JsonParseNew::check_exceed_size(size_t index)
{
    if (index >= m_size)
    {
        m_is_valid = false;
        return true;
    }

    return false;
}

Json JsonParseNew::parse_object(size_t& start_pos)
{
    Json res;

    while (m_object_string[start_pos] != '}')
    {
        std::string key = parse_key(start_pos);

        // Case: empty object
        if (key.data() == "}")
        {
            return res;
        }

        Json value = parse_value(start_pos);
        res[std::string(key.data())] = value;

        if (check_exceed_size(start_pos)) break;
    }

    start_pos++;

    return res;
}

Json JsonParseNew::parse_array(size_t& start_pos)
{
    Json res;
    int index = 0;

    while (m_object_string[start_pos] != ']')
    {
        Json value = parse_value(start_pos);
        res[index++] = value;

        if (check_exceed_size(start_pos)) break;
    }

    start_pos++;

    return res;
}

std::string JsonParseNew::parse_key(size_t& start_pos)
{
    // Find first character '\"'
    size_t start = start_pos;
    while (m_object_string[start] != '\"' && m_object_string[start] != '}')
    {
        start++;
        if (check_exceed_size(start)) break;
    }

    // Case: empty object
    if (m_object_string[start] == '}')
    {
        start_pos = start + 1;
        return "}";
    }

    start++;

    // Find second character '\"'
    size_t end = start;
    while (m_object_string[end] != '\"')
    {
        end++;
        if (check_exceed_size(end)) break;
    }

    start_pos = end + 1;

    return std::string(get_sub_string(start, end).data());
}

Json JsonParseNew::parse_value(size_t& start_pos)
{
    Json res;
    size_t start = start_pos;

    while (m_object_string[start] != '\"' && m_object_string[start] != '{' &&
        m_object_string[start] != '[' &&
        is_number(m_object_string[start]) == false &&
        is_boolean(m_object_string.data(), start, m_size) == NOT_BOOLEAN_VALUE &&
        is_null(m_object_string.data(), start, m_size) == false)
    {
        if (m_object_string[start] == '}' || m_object_string[start] == ']')
        {
            start_pos = start;
            break;
        }

        start++;
        if (check_exceed_size(start)) break;
    }

    if (m_is_valid == false) return res;

    // String
    if (m_object_string[start] == '\"')
    {
        start_pos = start + 1;
        res = parse_value_string(start_pos);
    }
    // Number
    else if (is_number(m_object_string[start]) == true)
    {
        start_pos = start;
        res = parse_value_number(start_pos);
    }
    // Boolean
    else if (int bool_value = is_boolean(m_object_string.data(), start, m_size))
    {
        if (bool_value == TRUE_VALUE)
        {
            res = true;
            start_pos = start + 4;
        }
        else if (bool_value == FALSE_VALUE)
        {
            res = false;
            start_pos = start + 5;
        }
    }
    // Null
    else if (is_null(m_object_string.data(), start, m_size))
    {
        res = nullptr; // If null, return a null Json
        start_pos = start + 4;
    }
    // Object
    else if (m_object_string[start] == '{')
    {
        start_pos = start + 1;
        res = parse_object(start_pos);
    }
    // Array
    else if (m_object_string[start] == '[')
    {
        start_pos = start + 1;
        res = parse_array(start_pos);
    }

    // Find character ',' or '}'
    while (m_object_string[start_pos] != ',' && m_object_string[start_pos] != '}' && m_object_string[start_pos] != ']')
    {
        start_pos++;
        if (check_exceed_size(start_pos)) break;
    }

    return res;
}

ShareString JsonParseNew::parse_value_string(size_t& start_pos)
{
    size_t start = start_pos;

    // Find end character '\"'
    size_t end = start_pos;
    while (m_object_string[end] != '\"')
    {
        end++;
        if (check_exceed_size(end)) break;
    }

    start_pos = end + 1;

    return get_sub_string(start, end);
}

Json JsonParseNew::parse_value_number(size_t& start_pos)
{
    Json res;
    size_t start = start_pos;
    bool is_float = false;

    // Find end number
    size_t end = start_pos;
    while (is_number(m_object_string[end]) || m_object_string[end] == '.' || (m_object_string[end] == 'e' && m_object_string[end + 1] == '-'))
    {
        if (m_object_string[end] == '.') is_float = true;

        end++;
        if (check_exceed_size(end)) break;
    }

    start_pos = end;

    std::string_view number_string = get_sub_string(start, end).data();
    if (is_float == false)
    {
        int64_t int_number = 0;
        auto [ptr, ec] = std::from_chars(number_string.data(), number_string.data() + number_string.size(), int_number);
        if (ec == std::errc())
        {
            // Success
            res = int_number;
        }
    }
    else
    {
        double double_number = 0.0;
        auto [ptr, ec] = std::from_chars(number_string.data(), number_string.data() + number_string.size(), double_number);
        if (ec == std::errc())
        {
            // Success
            res = double_number;
        }
    }

    return res;
}