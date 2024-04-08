#include "json_utils.h"
#include "utils.h"
#define STR2ULL(str) (base::utils::IsDigit(str) ? stoull(str) : 0)
#define STR2LL(str) (base::utils::IsDigit(str) ? stoll(str) : 0)
#define STR2I(str) (base::utils::IsDigit(str) ? stoi(str) : 0)
namespace wxhelper {
namespace jsonutils {
std::wstring GetWStringParam(nlohmann::json data, std::string key) {
  return base::utils::Utf8ToWstring(data[key].get<std::string>());
}

std::vector<std::wstring> GetArrayParam(nlohmann::json data, std::string key) {
  std::vector<std::wstring> result;
  std::wstring param = GetWStringParam(data, key);
  result = base::utils::split(param, L',');
  return result;
}

int GetIntParam(nlohmann::json data, std::string key) {
  int result;
  try {
    result = data[key].get<int>();
  } catch (nlohmann::json::exception) {
    result = STR2I(data[key].get<std::string>());
  }
  return result;
}

bool GetBoolParam(nlohmann::json data, std::string key) {
  return data[key].get<bool>();
}

std::string GetStringParam(nlohmann::json data, std::string key) {
  return data[key].get<std::string>();
}

int64_t GetInt64Param(nlohmann::json data, std::string key) {
  int64_t result;
  try {
    result = data[key].get<int64_t>();
  } catch (nlohmann::json::exception) {
    result = STR2LL(data[key].get<std::string>());
  }
  return result;
}
}  // namespace jsonutils
}  // namespace wxhelper
