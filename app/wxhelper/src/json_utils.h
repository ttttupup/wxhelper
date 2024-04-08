#ifndef WXHELPER_JSON_UTILS_H_
#define WXHELPER_JSON_UTILS_H_
#include "nlohmann/json.hpp"
namespace wxhelper {
namespace jsonutils {
    std::wstring GetWStringParam(nlohmann::json data, std::string key);
    std::vector<std::wstring> GetArrayParam(nlohmann::json data, std::string key) ;
    int GetIntParam(nlohmann::json data, std::string key);
    bool GetBoolParam(nlohmann::json data, std::string key);
    std::string GetStringParam(nlohmann::json data, std::string key);
    int64_t GetInt64Param(nlohmann::json data, std::string key);
}
}
#endif