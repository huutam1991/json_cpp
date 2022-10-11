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

    // Print out example
    std::cout << "json = " << json << std::endl;

    std::cout << "Main exit" << std::endl;

    return EXIT_SUCCESS;
}
