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
    Json data;
    data["type"] = "Car";
    std::cout << "data = " << data << std::endl;

    // Set/Get value
    Json value;
    value["int"] = -512342;
    value["float"] = 1231.242;
    value["string"] = std::string("Text");
    value["string2"] = "Text 2";
    value["bool"] = true;
    value["json"] = {"author", "TamNguyen"};
    value["array"] = Json::create_array();

    long a = value["int"];
    double b = value["float"];
    std::string s1 = value["string"];
    std::string s2 = value["string2"];
    bool d = value["bool"];
    Json object = value["json"];
    Json array = value["array"];

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "s1 = " << s1 << std::endl;
    std::cout << "s2 = " << s2 << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "object = " << object << std::endl;
    std::cout << "array = " << array << std::endl;

    // Check / remove field
    Json fields = {
        {"field1", 987},
        {"field2", 351},
    };
    std::cout << "fields = " << fields << std::endl; // {"field2":351,"field1":987}
    bool has_field_1 = fields.has_field("field1"); // true
    bool has_field_3 = fields.has_field("field3"); // false

    fields.remove_field("field1");
    std::cout << "fields = " << fields << std::endl; // {"field2":351}

    // Increase/decrease by value (for type number only)
    Json number;
    number["v"] = 1;
    std::cout << "number = " << number << std::endl; // {"v":1}
    number["v"] += 2;
    std::cout << "number = " << number << std::endl; // {"v":3}
    number["v"] -= 3;
    std::cout << "number = " << number << std::endl; // {"v":0}

    // Loop through child
    Json loop = {
        {"child1", 1},
        {"child2", 2},
        {"child3", 3},
        {"child4", 4},
        {"child5", 5},
    };
    loop.for_each([](Json& child)
    {
        std::cout << "child = " << child << std::endl;
    });

    // Loop through child with key
    loop.for_each_with_key([](const std::string& key, Json& child)
    {
        std::cout << "key = " << key << std::endl;
        std::cout << "child = " << child << std::endl;
    });

    // Parse from string
    Json p = Json::parse("{\"value\": 345}");
    std::cout << "p = " << p << std::endl; // {"value":345}

    // Json array
    Json arr = Json::create_array();
    std::cout << "array = " << arr << std::endl; // []
    arr[0] = 234;
    arr[1] = 34.58;
    arr[2] = false;
    arr[3] = true;
    arr[4] = "TamNguyen";
    arr[5] = {"color", "green"};
    std::cout << "array = " << arr << std::endl; // [234,34.58,false,true,"TamNguyen",{"color":"green"}]

    // Loop through array with index
    arr.for_each_with_index([](size_t index, Json& child)
    {
        std::cout << "index = " << index << std::endl;
        std::cout << "child = " << child << std::endl;
    });

    // Sort array
    Json arr_sort = Json::create_array();
    arr_sort[0] = {"value", 712};
    arr_sort[1] = {"value", 4334};
    arr_sort[2] = {"value", 132};
    std::cout << "arr_sort = " << arr_sort << std::endl; // [{"value":712},{"value":4334},{"value":132}]

    arr_sort.sort([](Json& a, Json& b) -> bool
    {
        return (int)a["value"] < (int)b["value"];
    });
    std::cout << "arr_sort = " << arr_sort << std::endl; // [{"value":132},{"value":712},{"value":4334}]

    std::cout << "Main exit" << std::endl;

    return EXIT_SUCCESS;
}
