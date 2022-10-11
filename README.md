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