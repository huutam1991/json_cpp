#include <json/json.h>

Json::Json() : m_value(std::make_shared<JsonObject>()) // Json contains a json object by default
{
}

Json::Json(const Json& copy_json)
{
    m_value = copy_json.m_value.get()->get_clone(copy_json.m_value);
}


Json::Json(std::initializer_list<Json> json_list)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();

    for (auto it = json_list.begin(); it != json_list.end(); it++)
    {
        const Json& json = *it;
        json.for_each_with_key([&json_object](const std::string& key, Json& data)
        {
            json_object[key] = data;
        });
    }
}

Json::Json(const std::string& key, const Json& copy_json)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object[key] = copy_json;
}

Json::Json(const JsonNull& json_null)
{
    m_value = std::make_shared<JsonValue<JsonNull>>(json_null);
}

Json& Json::operator=(const Json& copy_json)
{
    m_value = copy_json.m_value.get()->get_clone(copy_json.m_value);
    return *this;
}

std::ostream& operator<<(std::ostream& cout, const Json& json)
{
    cout << json.get_string_value();
    return cout;
}

const std::string Json::get_string_value() const
{
    return m_value.get()->get_string_value();
}

void Json::remove_field(const std::string& field)
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        ptr_cast->remove_field(field);
    }
}

bool Json::has_field(const std::string& field) const
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        return ptr_cast->has_field(field);
    }

    return false;
}

bool Json::is_array() const
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        return ptr_cast->is_array();
    }

    return false;
}

bool Json::is_null() const
{
    JsonValue<JsonNull>* ptr_cast = dynamic_cast<JsonValue<JsonNull>*>(m_value.get());
    return ptr_cast != nullptr;
}

void Json::set_size(size_t size)
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        ptr_cast->set_size(size);
    }
}

int Json::size() const
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        return ptr_cast->size();
    }

    return 0;
}

Json& Json::operator[](const std::string& key)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object.m_is_object = true; // make this json an object

    return json_object[key];
}

Json& Json::operator[](const char* key_char)
{
    return operator[](std::string(key_char));
}

Json& Json::operator[](int index)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object.m_is_object = false; // make this json an array

    return json_object[index];
}

template<>
void Json::push_back(const Json& value)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object.push_back(value);
}

void Json::for_each(std::function<void(Json&)> loop_func) const
{
    // Only JsonObject is able to loop as it has child elements
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        // Loop through object
        if (ptr_cast->m_is_object)
        {
            for (auto it = ptr_cast->m_object.begin(); it != ptr_cast->m_object.end(); it++)
            {
                loop_func(it->second);
            }
        }
        // Loop through array
        else
        {
            for (auto it = ptr_cast->m_array.begin(); it != ptr_cast->m_array.end(); it++)
            {
                loop_func(*it);
            }
        }
    }
}

void Json::for_each_with_key(std::function<void(const std::string&, Json&)> loop_func) const
{
    // Only JsonObject is able to loop as it has child elements
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        // Loop through object
        if (ptr_cast->m_is_object)
        {
            for (auto it = ptr_cast->m_object.begin(); it != ptr_cast->m_object.end(); it++)
            {
                loop_func(it->first, it->second);
            }
        }
    }
}

void Json::for_each_with_index(std::function<void(size_t,Json&)> loop_func) const
{
    // Only JsonObject is able to loop as it has child elements
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        // Loop through array
        if (ptr_cast->m_is_object == false)
        {
            for (size_t i = 0; i < ptr_cast->m_array.size(); i++)
            {
                loop_func(i, ptr_cast->m_array[i]);
            }
        }
    }
}

void Json::sort(std::function<bool(Json&,Json&)> compare_func)
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(m_value.get());
    if (ptr_cast != nullptr && ptr_cast->is_array())
    {
        std::sort(ptr_cast->m_array.begin(), ptr_cast->m_array.end(), compare_func);
    }
}

void Json::check_create_json_object(Json& json)
{
    JsonObject* ptr_cast = dynamic_cast<JsonObject*>(json.m_value.get());
    if (ptr_cast == nullptr)
    {
        json.m_value = std::make_shared<JsonObject>();
    }
}

Json Json::parse(const std::string& json_string)
{
    JsonParse json_parse;
    return json_parse.parse(json_string);;
}

Json Json::create_array()
{
    Json res;
    res.check_create_json_object(res); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)res.m_value.get();
    json_object.m_is_object = false; // make this json an array
    return res;
}

std::stringstream& operator<<(std::stringstream& ss, const JsonNull& json_null)
{
    return ss;
}

template<>
Json::operator int()
{
    bool is_castable = false;

    // long
    long long_val = check_cast_value_by_type<long>(is_castable);
    if (is_castable)
    {
        return (int)long_val;
    }

    // long long
    long long long_long_val = check_cast_value_by_type<long long>(is_castable);
    if (is_castable)
    {
        return (int)long_long_val;
    }

    // size_t
    size_t size_t_val = check_cast_value_by_type<size_t>(is_castable);
    if (is_castable)
    {
        return (int)size_t_val;
    }

    return *(JsonValue<int>*)m_value.get();
}

template<>
Json::operator long()
{
    bool is_castable = false;

    // int
    int int_val = check_cast_value_by_type<int>(is_castable);
    if (is_castable)
    {
        return (long)int_val;
    }

    // long long
    long long long_long_val = check_cast_value_by_type<long long>(is_castable);
    if (is_castable)
    {
        return (long)long_long_val;
    }

    // size_t
    size_t size_t_val = check_cast_value_by_type<size_t>(is_castable);
    if (is_castable)
    {
        return (long)size_t_val;
    }

    return *(JsonValue<long>*)m_value.get();
}

template<>
Json::operator long long()
{
    bool is_castable = false;

    // int
    int int_val = check_cast_value_by_type<int>(is_castable);
    if (is_castable)
    {
        return (long long)int_val;
    }

    // long
    long long_val = check_cast_value_by_type<long>(is_castable);
    if (is_castable)
    {
        return (long long)long_val;
    }

    // size_t
    size_t size_t_val = check_cast_value_by_type<size_t>(is_castable);
    if (is_castable)
    {
        return (long long)size_t_val;
    }

    return *(JsonValue<long long>*)m_value.get();
}

template<>
Json::operator size_t()
{
    bool is_castable = false;

    // int
    int int_val = check_cast_value_by_type<int>(is_castable);
    if (is_castable)
    {
        return (size_t)int_val;
    }

    // long
    long long_val = check_cast_value_by_type<long>(is_castable);
    if (is_castable)
    {
        return (size_t)long_val;
    }

    // long long
    long long long_long_val = check_cast_value_by_type<long long>(is_castable);
    if (is_castable)
    {
        return (size_t)long_long_val;
    }

    return *(JsonValue<size_t>*)m_value.get();
}

template<>
Json::operator float()
{
    bool is_castable = false;

    // double
    double double_val = check_cast_value_by_type<double>(is_castable);
    if (is_castable)
    {
        return (float)double_val;
    }

    // long double
    long double long_double_val = check_cast_value_by_type<long double>(is_castable);
    if (is_castable)
    {
        return (float)long_double_val;
    }

    return *(JsonValue<float>*)m_value.get();
}

template<>
Json::operator double()
{
    bool is_castable = false;

    // float
    float float_val = check_cast_value_by_type<float>(is_castable);
    if (is_castable)
    {
        return (double)float_val;
    }

    // long double
    long double long_double_val = check_cast_value_by_type<long double>(is_castable);
    if (is_castable)
    {
        return (double)long_double_val;
    }

    return *(JsonValue<double>*)m_value.get();
}

template<>
Json::operator long double()
{
    bool is_castable = false;

    // float
    float float_val = check_cast_value_by_type<float>(is_castable);
    if (is_castable)
    {
        return (long double)float_val;
    }

    // double
    double double_val = check_cast_value_by_type<double>(is_castable);
    if (is_castable)
    {
        return (long double)double_val;
    }

    return *(JsonValue<long double>*)m_value.get();
}

template<>
Json::operator std::string()
{
    // const char
    JsonValue<const char*>* ptr_c = dynamic_cast<JsonValue<const char*>*>(m_value.get());
    if (ptr_c != nullptr)
    {
        const char* value = *ptr_c;
        return std::string(value);
    }

    // string
    JsonValue<std::string>* ptr_s = dynamic_cast<JsonValue<std::string>*>(m_value.get());
    if (ptr_s != nullptr)
    {
        return *(JsonValue<std::string>*)m_value.get();
    }

    return std::string();
}