#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <unordered_map>
#include <vector>

#include "json_type_base.h"
#include "json_define.h"

class JsonObject : public JsonTypeBase
{
private:
    bool m_is_object = true;

    std::unordered_map<std::string, Json> m_object;
    std::vector<Json> m_array;

public:
    JsonObject();
    virtual ~JsonObject();

    // Inheritance methods
    virtual bool is_json_value() { return false; }
    virtual const std::string get_string_value() const;
    virtual std::shared_ptr<JsonTypeBase> get_clone(const std::shared_ptr<JsonTypeBase>&);

    Json& operator[](const std::string&);
    Json& operator[](int);

    void remove_field(const std::string& field);
    bool has_field(const std::string& field);
    bool is_array();
    void set_size(size_t size);
    int size();

    template<class T>
    void push_back(const T& value);

    friend class Json;
};

template<class T>
void JsonObject::push_back(const T& value)
{
    Json data;
    data = value;
    m_array.push_back(data);
}

template<class T>
void Json::push_back(const T& value)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object.push_back(value);
}

template<class T>
Json::Json(const std::string& key, const T& value)
{
    check_create_json_object(*this); // Create JsonObject if it does not exist
    JsonObject& json_object = *(JsonObject*)m_value.get();
    json_object[key] = value;
}

#endif //JSON_OBJECT_H