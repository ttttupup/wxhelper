#include "http_url_handler.h"

#include <nlohmann/json.hpp>

#include "utils.h"
#include "wechat_service.h"
#include "wechat_hook.h"

#define STR2ULL(str) (base::utils::IsDigit(str) ? stoull(str) : 0)
#define STR2LL(str) (base::utils::IsDigit(str) ? stoll(str) : 0)
#define STR2I(str) (base::utils::IsDigit(str) ? stoi(str) : 0)
namespace wxhelper {
std::wstring GetWStringParam(nlohmann::json data, std::string key) {
  return base::utils::Utf8ToWstring(data[key].get<std::string>());
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

std::string HookSyncMsg(mg_http_message* hm) {
  nlohmann::json j_param = nlohmann::json::parse(
      hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  int port = GetIntParam(j_param, "port");
  std::string ip = GetStringParam(j_param, "ip");
  bool enable = GetBoolParam(j_param, "enableHttp");
  std::string url = "http:://127.0.0.1:19088";
  uint64_t timeout = 3000;
  if (enable) {
    url = GetStringParam(j_param, "url");
    timeout = GetIntParam(j_param, "timeout");
  }
  hook::WechatHookParam param = {
      ip, url, port, enable, timeout,
  };
  hook::WechatHook::GetInstance().Init(param);
  INT64 success = hook::WechatHook::GetInstance().HookSyncMsg();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  std::string ret = ret_data.dump();
  return ret;
}

std::string GetContacts(mg_http_message* hm) {
  std::vector<common::ContactInner> vec;
  INT64 success =  WechatService::GetInstance().GetContacts(vec);
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  for (unsigned int i = 0; i < vec.size(); i++) {
    nlohmann::json item = {
        {"customAccount", vec[i].custom_account},
        {"encryptName", vec[i].encrypt_name},
        {"type", vec[i].type},
        {"verifyFlag", vec[i].verify_flag},
        {"wxid", vec[i].wxid},
        {"nickname", vec[i].nickname},
        {"pinyin", vec[i].pinyin},
        {"pinyinAll", vec[i].pinyin_all},
        {"reserved1", vec[i].reserved1},
        {"reserved2", vec[i].reserved2},
    };
    ret_data["data"].push_back(item);
  }
  std::string ret = ret_data.dump();
  return ret;
}
std::string UnHookSyncMsg(mg_http_message* hm) {
  INT64 success = hook::WechatHook::GetInstance().UnHookSyncMsg();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}
}  // namespace wxhelper