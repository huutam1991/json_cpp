# json_cpp
Very simple, very fast Json for C++

# Usage
Very simple, just copy files from 2 folders **include/json** and **src/json** to your project, then put **#include <json/json.h>** to the file's header (pls take a look at **main.cpp**)

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
Get string value
```cpp
Json json;
json["type"] = "Car";
std::string json_str = json.get_string_value();
```
Parse from string
```cpp
Json parse = Json::parse("{\"value\": 345}");
std::cout << "parse = " << parse << std::endl; // {"value":345}
```
Set value (you can put value of any type as you wish)
```cpp
Json value;
value["int"] = 5142;
value["float"] = (float)1231.242;
value["string"] = std::string("Text");
value["string2"] = "Text 2";
value["bool"] = true;
value["json"] = {"author", "TamNguyen"};
```
Get value (pls specific the type of value)
```cpp
long a = value["int"];
long double b = value["float"];
std::string s1 = value["string"];
std::string s2 = value["string2"];
bool d = value["bool"];
Json object = value["json"];
```
Increase/decrease by value (for type number only)
```cpp
Json number;
number["v"] = 1;
std::cout << "number = " << number << std::endl; // {"v":1}
number["v"] += 2;
std::cout << "number = " << number << std::endl; // {"v":3}
number["v"] -= 3;
std::cout << "number = " << number << std::endl; // {"v":0}
```
Loop through child
```cpp
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
```
Loop through child with key
```cpp
loop.for_each_with_key([](const std::string& key, Json& child)
{
  std::cout << "key = " << key << std::endl;
  std::cout << "child = " << child << std::endl;
});
```
