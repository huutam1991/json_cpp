#include <utility>

#include <json/json_object.h>
#include <json/json_value.h>

JsonObject::JsonObject()
{
}

JsonObject::~JsonObject()
{
}

Json& get_object_value(const std::string& key, std::unordered_map<std::string, Json>& object)
{
    auto it = object.find(key);

    if (it != object.end())
    {
        return it->second;
    }
    else
    {
        object.insert(std::make_pair(key, Json()));
        return object[key];
    }
}

Json& get_object_value(size_t index, std::vector<Json>& array)
{
    if (index >= array.size())
    {
        array.resize(index + 1);
    }

    return array[index];
}

Json& JsonObject::operator[](const std::string& key)
{
    return get_object_value(key, m_object);
}

Json& JsonObject::operator[](int index)
{
    return get_object_value(index, m_array);
}

const std::string JsonObject::get_string_value() const
{
    std::string res;
    size_t size = m_is_object ? m_object.size() : m_array.size();
    int counter = 0;

    if (m_is_object == true)
    {
        res.append("{");
        for (auto it = m_object.begin(); it != m_object.end(); it++)
        {
            res.append("\"" + it->first + "\"");
            res.append(":");
            res.append(it->second.get_string_value());

            if (++counter < size) res.append(",");
        }
        res.append("}");
    }
    else
    {
        res.append("[");
        for (int i = 0; i < size; i++)
        {
            res.append(m_array[i].get_string_value());

            if (++counter < size) res.append(",");
        }
        res.append("]");
    }

    return res;
}

std::shared_ptr<JsonTypeBase> JsonObject::get_clone(const std::shared_ptr<JsonTypeBase>& ptr)
{
    std::shared_ptr<JsonTypeBase> res = ptr;
    return res;
}

void JsonObject::remove_field(const std::string& field)
{
    if (m_is_object == true)
    {
        m_object.erase(field);
    }
}

bool JsonObject::has_field(const std::string& field)
{
    if (m_is_object == true)
    {
        auto it = m_object.find(field);
        return it != m_object.end();
    }

    return false;
}

bool JsonObject::is_array()
{
    return !m_is_object;
}

void JsonObject::set_size(size_t size)
{
    m_array.resize(size);
}

int JsonObject::size()
{
    return m_array.size();
}
