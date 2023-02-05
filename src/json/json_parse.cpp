#include <string.h>

#include <json/json_parse.h>

#define TRUE_VALUE 1
#define FALSE_VALUE 2
#define NOT_BOOLEAN_VALUE 0

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


std::string get_sub_string(const char* m_object_string, size_t start, size_t end)
{
    size_t size = end - start;

    char s[size + 1];
    memcpy(s, m_object_string + start, size);
    s[size] = '\0'; // Add character '\0' at the end

    return std::string(s);
}

Json JsonParse::parse(const std::string& object)
{
    Json res;
    m_object_string = object.c_str();
    m_size = object.size();

    if (m_size == 0)
    {
        res = JsonNull();
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
        res = JsonNull();
    }

    return res;
}

bool JsonParse::check_exceed_size(size_t index)
{
    if (index >= m_size)
    {
        m_is_valid = false;
        return true;
    }

    return false;
}

Json JsonParse::parse_object(size_t& start_pos)
{
    Json res;

    while (m_object_string[start_pos] != '}')
    {
        std::string key = parse_key(start_pos);

        // Case: empty object
        if (key == "}")
        {
            return res;
        }

        Json value = parse_value(start_pos);
        res[key] = value;

        if (check_exceed_size(start_pos)) break;
    }

    start_pos++;

    return res;
}

Json JsonParse::parse_array(size_t& start_pos)
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

std::string JsonParse::parse_key(size_t& start_pos)
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

    return get_sub_string(m_object_string, start, end);
}

Json JsonParse::parse_value(size_t& start_pos)
{
    Json res;
    size_t start = start_pos;

    while (m_object_string[start] != '\"' && m_object_string[start] != '{' &&
        m_object_string[start] != '[' &&
        is_number(m_object_string[start]) == false &&
        is_boolean(m_object_string, start, m_size) == NOT_BOOLEAN_VALUE &&
        is_null(m_object_string, start, m_size) == false)
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
    else if (int bool_value = is_boolean(m_object_string, start, m_size))
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
    else if (is_null(m_object_string, start, m_size))
    {
        res = JsonNull();
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

std::string JsonParse::parse_value_string(size_t& start_pos)
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

    return get_sub_string(m_object_string, start, end);
}

Json JsonParse::parse_value_number(size_t& start_pos)
{
    Json res;
    size_t start = start_pos;
    bool is_float = false;

    // Find end number
    size_t end = start_pos;
    while (is_number(m_object_string[end]) || m_object_string[end] == '.')
    {
        if (m_object_string[end] == '.') is_float = true;

        end++;
        if (check_exceed_size(end)) break;
    }

    start_pos = end;

    std::string number_string = get_sub_string(m_object_string, start, end);
    if (is_float)
    {
        res = std::stold(number_string);
    }
    else
    {
        res = std::stol(number_string);
    }

    return res;
}