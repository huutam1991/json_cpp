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
    std::cout << "json = " << json3 << std::endl;

    std::cout << "Main exit" << std::endl;

    return EXIT_SUCCESS;
}
