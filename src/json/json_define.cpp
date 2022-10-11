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

std::stringstream& operator<<(std::stringstream& ss, const JsonNull& json_null)
{
    return ss;
}

template<>
Json::operator int()
{
    return *(JsonValue<int>*)m_value.get();
}

template<>
Json::operator long()
{
    return *(JsonValue<long>*)m_value.get();
}

template<>
Json::operator long long()
{
    return *(JsonValue<long long>*)m_value.get();
}

template<>
Json::operator size_t()
{
    return *(JsonValue<size_t>*)m_value.get();
}

template<>
Json::operator float()
{
    return *(JsonValue<float>*)m_value.get();
}

template<>
Json::operator double()
{
    return *(JsonValue<double>*)m_value.get();
}

template<>
Json::operator long double()
{
    return *(JsonValue<long double>*)m_value.get();
}