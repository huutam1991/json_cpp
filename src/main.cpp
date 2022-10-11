#include <iostream>

#include <json/json.h>

int main(int argc, char **argv)
{
    // Create example
    Json json = {
        {"currency", "USDT"},
        {"value", 24.55},
        {
            "user_info", {
                {"user_name", "TamNguyen"},
                {"team_size", 1},
                {"is_developer", true},
                {"is_devops", false}
            }
        }
    };

    Json json2;
    json2["currency"] = "USDT";
    json2["value"] = "24.55";
    json2["user_info"] = {
        {"user_name", "TamNguyen"},
        {"team_size", 1},
        {"is_developer", true},
        {"is_devops", false}
    };

    Json json3 = {"tam", 131.2342};

    // Print out example
    std::cout << "json = " << json << std::endl;
    std::cout << "json = " << json2 << std::endl;
    std::cout << "json = " << json3.get_string_value() << std::endl;

    // Set/Get value
    Json value;
    value["int"] = 5142;
    value["float"] = (float)1231.242;
    value["string"] = std::string("Text");
    value["string2"] = "Text 2";
    value["bool"] = true;
    value["json"] = {"author", "TamNguyen"};

    long a = value["int"];
    long double b = value["float"];
    std::string s1 = value["string"];
    std::string s2 = value["string2"];
    bool d = value["bool"];
    Json object = value["json"];

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "s1 = " << s1 << std::endl;
    std::cout << "s2 = " << s2 << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "object = " << object << std::endl;

    // Increase/decrease by value (for type number only)
    Json number;
    number["v"] = 1;
    std::cout << "number = " << number << std::endl; // {"v":1}
    number["v"] += 2;
    std::cout << "number = " << number << std::endl; // {"v":3}
    number["v"] -= 3;
    std::cout << "number = " << number << std::endl; // {"v":0}

    // Loop through child

    std::cout << "Main exit" << std::endl;

    return EXIT_SUCCESS;
}
