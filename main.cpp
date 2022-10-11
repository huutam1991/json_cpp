#include <iostream>
#include <filesystem>

#include <json/json.h>

int main(int argc, char **argv)
{
    Json a = {
        {"currency", "USDT"},
        {"value", 24.55}
    };

    std::cout << "Main exit" << std::endl;

    return EXIT_SUCCESS;
}
