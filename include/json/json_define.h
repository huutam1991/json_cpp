#ifndef JSON_DEFINE_H
#define JSON_DEFINE_H

#include <memory>
#include <functional>
#include <type_traits>

#include "json_type_base.h"
#include "json_value.h"

struct JsonNull;

class Json
{
private:
    std::shared_ptr<JsonTypeBase> m_value = nullptr;

    void check_create_json_object(Json& json);
    template<class T>
    T check_cast_value_by_type(bool& is_castable);

public:
    Json();
    Json(const Json&);
    Json(const JsonNull&);
    Json(std::initializer_list<Json>);
    Json(const std::string& key, const Json& copy_json);

    template<class T>
    Json(const std::string& key, const T& value);

    friend std::ostream& operator<<(std::ostream& cout, const Json& json);
    static Json parse(const std::string&);
    static Json create_array();

    const std::string get_string_value() const;
    void remove_field(const std::string& field);
    bool has_field(const std::string& field) const;
    bool is_array() const;
    bool is_null() const;
    void set_size(size_t size);
    int size() const;
    void for_each(std::function<void(Json&)> loop_func) const;
    void for_each_with_key(std::function<void(const std::string&,Json&)> loop_func) const;
    void for_each_with_index(std::function<void(size_t,Json&)> loop_func) const;
    void sort(std::function<bool(Json&,Json&)>);

    Json& operator=(const Json&);
    Json& operator[](const std::string&);
    Json& operator[](const char*);
    Json& operator[](int);

    template<class T>
    Json& operator=(T value);

    template<class T>
    operator T();

    template<class T>
    bool operator ==(T value);

    template<class T>
    bool operator !=(T value);

    template<class T>
    Json& operator +=(T value);

    template<class T>
    Json& operator -=(T value);

    template<class T>
    void push_back(const T& value);
};

template<class T>
Json& Json::operator=(T value)
{
    // If the new value has the same type of current value, just update it
    JsonValue<T>* ptr_cast = dynamic_cast<JsonValue<T>*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        ptr_cast->update_value(value);
    }
    else // ortherwise, create a new JsonValue<T>
    {
        m_value = std::make_shared<JsonValue<T>>(value);
    }

    return *this;
}

template<class T>
Json::operator T()
{
    // If conversion value has the same type of current value, return it
    JsonValue<T>* ptr_cast = dynamic_cast<JsonValue<T>*>(m_value.get());
    if (ptr_cast != nullptr)
    {
        return *(JsonValue<T>*)m_value.get();
    }
    else // ortherwise, return a random T conversion value
    {
        return T();
    }
}

template<class T>
bool Json::operator==(T value)
{
    T json_value = Json::operator T();
    return json_value == value;
}

template<class T>
bool Json::operator!=(T value)
{
    T json_value = Json::operator T();
    return json_value != value;
}

template<class T>
Json& Json::operator+=(T value)
{
    // If the adding value has different type of current value, create a new one
    JsonValue<T>* ptr_cast = dynamic_cast<JsonValue<T>*>(m_value.get());
    if (ptr_cast == nullptr)
    {
        m_value = std::make_shared<JsonValue<T>>(value);
    }
    // otherwise just add the value
    else
    {
        ptr_cast->add_value(value);
    }

    return *this;
}

template<class T>
Json& Json::operator-=(T value)
{
    // If the sub value has different type of current value, create a new one
    JsonValue<T>* ptr_cast = dynamic_cast<JsonValue<T>*>(m_value.get());
    if (ptr_cast == nullptr)
    {
        m_value = std::make_shared<JsonValue<T>>(value);
    }
    // otherwise just sub the value
    else
    {
        ptr_cast->sub_value(value);
    }

    return *this;
}

template<class T>
T Json::check_cast_value_by_type(bool& is_castable)
{
    JsonValue<T>* ptr = dynamic_cast<JsonValue<T>*>(m_value.get());
    if (ptr != nullptr)
    {
        is_castable = true;
        return (T)*ptr;
    }

    is_castable = false;
    return T();
}

struct JsonNull
{
public:
    friend std::stringstream& operator<<(std::stringstream& ss, const JsonNull& json_null);
};

#endif //JSON_DEFINE_H