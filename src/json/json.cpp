#include <json/json.h>
#include <json/json_object.h>
#include <json/json_value.h>
#include <json/json_parse.h>

Json::Json()
{}

Json::Json(std::initializer_list<std::pair<std::string, Json>> json_list)
{
    check_create_json_object(); // Create JsonObject if it does not exist
    JsonObject* json_object = (JsonObject*)m_value;

    for (auto& pair : json_list)
    {
        json_object->add_pair(pair);
    }
}

Json Json::parse(std::string json_string)
{
    JsonParseNew parser(std::move(json_string));
    return parser.parse();
}

Json& Json::operator[](const char* key)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    return (*json_object)[key];
}

Json& Json::operator[](const std::string& key)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    return (*json_object)[key.c_str()];
}

Json& Json::operator[](size_t index)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    return (*json_object)[index];
}

void Json::for_each(std::function<void(Json&)> loop_func)
{
    if (m_value == nullptr)
    {
        return; // If m_value is null, do nothing
    }

    if (m_value->is_json_value() == false)
    {
        static_cast<JsonObject*>(m_value)->for_each(loop_func);
    }
}

void Json::for_each_with_key(std::function<void(const std::string&,Json&)> loop_func)
{
    if (m_value == nullptr)
    {
        return; // If m_value is null, do nothing
    }

    if (m_value->is_json_value() == false)
    {
        static_cast<JsonObject*>(m_value)->for_each_with_key(loop_func);
    }
}

void Json::for_each_with_index(std::function<void(size_t,Json&)> loop_func)
{
    if (m_value == nullptr)
    {
        return; // If m_value is null, do nothing
    }

    if (m_value->is_json_value() == false)
    {
        static_cast<JsonObject*>(m_value)->for_each_with_index(loop_func);
    }
}
Json::iterator Json::begin()
{
    check_create_json_object();
    return static_cast<JsonObject*>(m_value)->begin();
}

Json::iterator Json::end()
{
    check_create_json_object();
    return static_cast<JsonObject*>(m_value)->end();
}

Json::const_iterator Json::begin() const
{
    if (m_value && !m_value->is_json_value())
    {
        return ((const JsonObject*)m_value)->begin();
    }
    throw std::runtime_error("Not a JSON object");
}

Json::const_iterator Json::end() const
{
    if (m_value && !m_value->is_json_value())
    {
        return ((const JsonObject*)m_value)->end();
    }
    throw std::runtime_error("Not a JSON object");
}

bool Json::has_field(const std::string& field) const
{
    if (m_value->is_json_value() == false)
    {
        return static_cast<JsonObject*>(m_value)->has_field(field);
    }
    return false; // If it's a JsonValue, it cannot have fields
}

void Json::remove_field(const std::string& field)
{
    if (m_value->is_json_value() == false)
    {
        static_cast<JsonObject*>(m_value)->remove_field(field);
    }
}

bool Json::is_array() const
{
    return m_value == nullptr ?
        false :
        m_value->is_json_value() == false && static_cast<JsonObject*>(m_value)->is_array();
}

bool Json::is_object() const
{
    return m_value == nullptr ?
        false :
        m_value->is_json_value() == false && static_cast<JsonObject*>(m_value)->is_array() == false;
}

void Json::set_size(size_t size)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    json_object->set_size(size);
}

void Json::set_capacity(size_t size)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    json_object->set_capacity(size);
}

int Json::size() const
{
    return m_value->is_json_value() ? 0 : static_cast<JsonObject*>(m_value)->size();
}

int Json::capacity() const
{
    return m_value->is_json_value() ? 0 : static_cast<JsonObject*>(m_value)->capacity();
}

void Json::reverse()
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    json_object->reverse();
}

void Json::sort(std::function<bool(Json&, Json&)> compare_func)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    json_object->sort(compare_func);
}

void Json::push_back(const Json& value)
{
    check_create_json_object();
    JsonObject* json_object = (JsonObject*)m_value;
    json_object->push_back(value);
}

bool Json::is_null() const
{
    return m_value == nullptr || (m_value->is_json_value() && static_cast<JsonValue*>(m_value)->is_null());
}

void Json::check_create_json_value()
{
    if (m_value == nullptr)
    {
        m_value = JsonValuePool::acquire();
    }

    if (m_value->is_json_value() == false)
    {
        m_value->release();
        m_value = JsonValuePool::acquire();
    }
}

void Json::check_create_json_object()
{
    if (m_value == nullptr)
    {
        m_value = JsonObjectPool::acquire();
    }

    if (m_value->is_json_value() == true)
    {
        m_value->release();
        m_value = JsonObjectPool::acquire();
    }
}

std::ostream& operator<<(std::ostream& cout, const Json& json)
{
    cout << json.get_string_value();
    return cout;
}