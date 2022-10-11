# json_cpp
Very simple, very fast Json for C++

# Usage
Very simple, just copy files from 2 folders **include/json** + **src/json** to your project, then put **#include <json/json.h>** to the file's header (pls take a look at **main.cpp**)

# Example
Create a json object
```cpp
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
```
Or put a **value** using **key** (string)
```cpp
Json json;
json["currency"] = "USDT";
json["value"] = "24.55";
json["user_info"] = {
  {"user_name", "TamNguyen"},
  {"team_size", 1},
  {"is_developer", true},
  {"is_devops", false}
};
```
Print out console
```cpp
Json json;
json["type"] = "Car";
std::cout << "json = " << json << std::endl;
```