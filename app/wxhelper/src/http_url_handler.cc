#include "http_url_handler.h"

#include <nlohmann/json.hpp>

#include "utils.h"
#include "wechat_service.h"
#include "windows.h"
namespace wxhelper {
std::wstring GetWStringParam(nlohmann::json data, std::string key) {
  return base::utils::Utf8ToWstring(data[key].get<std::string>());
}

std::string SendTextMsg(mg_http_message* hm) {
  nlohmann::json j_param = nlohmann::json::parse(
      hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  std::wstring wxid = GetWStringParam(j_param, "wxid");
  std::wstring msg = GetWStringParam(j_param, "msg");
  INT64 success = WechatService::GetInstance().SendTextMsg(wxid, msg);
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  std::string ret = ret_data.dump();
  return ret;
}
}  // namespace wxhelper