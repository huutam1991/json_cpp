#ifndef JSON_TYPE_BASE_H
#define JSON_TYPE_BASE_H

#include <string>
#include <memory>

class JsonTypeBase
{
protected:
    JsonTypeBase(const JsonTypeBase&) = delete;
    JsonTypeBase() = default;

public:
    virtual ~JsonTypeBase() {};

    virtual bool is_json_value() = 0;
    virtual const std::string get_string_value() const = 0;
    virtual std::shared_ptr<JsonTypeBase> get_clone(const std::shared_ptr<JsonTypeBase>&) = 0;
};

#endif //JSON_TYPE_BASE_H
