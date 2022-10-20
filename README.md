# C++ Json
Very simple, very fast Json for C++

# Usage
Very simple, just copy 2 folders **include/json** and **src/json** to your project, then put **#include <json/json.h>** to the file's header (pls take a look at **src/main.cpp**). That's it !!!

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
json["value"] = 24.55;
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
Json p = Json::parse("{\"value\": 345}");
std::cout << "p = " << p << std::endl; // {"value":345}
```
Set value (you can put value of any type as you wish)
```cpp
Json value;
value["int"] = -512342;
value["float"] = 1231.242;
value["string"] = std::string("Text");
value["string2"] = "Text 2";
value["bool"] = true;
value["json"] = {"author", "TamNguyen"};
value["array"] = Json::create_array();
```
Get value (pls specific the type of value)
```cpp
long a = value["int"];
double b = value["float"];
std::string s1 = value["string"];
std::string s2 = value["string2"];
bool d = value["bool"];
Json object = value["json"];
Json array = value["array"];
```
Check / remove field
```cpp
Json fields = {
  {"field1", 987},
  {"field2", 351},
};
std::cout << "fields = " << fields << std::endl; // {"field2":351,"field1":987}
bool has_field_1 = fields.has_field("field1"); // true
bool has_field_3 = fields.has_field("field3"); // false

fields.remove_field("field1");
std::cout << "fields = " << fields << std::endl; // {"field2":351}
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
Json array
```cpp
Json array = Json::create_array();
std::cout << "array = " << array << std::endl; // []
array[0] = 234;
array[1] = 34.58;
array[2] = false;
array[3] = true;
array[4] = "TamNguyen";
array[5] = {"color", "green"};
std::cout << "array = " << array << std::endl; // [234,34.58,false,true,"TamNguyen",{"color":"green"}]
```
Or you can push elements to an array
```cpp
Json arr2 = Json::create_array();
std::cout << "array = " << arr2 << std::endl; // []
arr2.push_back(234);
arr2.push_back(34.58);
arr2.push_back(false);
arr2.push_back(true);
arr2.push_back("TamNguyen");
arr2.push_back(Json{"color", "green"});
std::cout << "array = " << arr2 << std::endl; // [234,34.58,false,true,"TamNguyen",{"color":"green"}]
```
Loop through array with index
```cpp
arr.for_each_with_index([](size_t index, Json& child)
{
    std::cout << "index = " << index << std::endl;
    std::cout << "child = " << child << std::endl;
});
```
Sort array
```cpp
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
```
