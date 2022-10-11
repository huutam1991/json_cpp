#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include "json_define.h"

class JsonParse
{
private:
    const char* m_object_string;
    size_t m_size;
    bool m_is_valid = true;

    bool check_exceed_size(size_t index);

public:
    Json parse(const std::string& object);
    Json parse_object(size_t& start_pos);
    Json parse_array(size_t& start_pos);
    Json parse_value_number(size_t& start_pos);
    Json parse_value(size_t& start_pos);
    std::string parse_key(size_t& start_pos);
    std::string parse_value_string(size_t& start_pos);
};

#endif //JSON_PARSE_H