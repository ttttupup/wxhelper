#include "http_url_handler.h"

#include <nlohmann/json.hpp>

#include "db.h"
#include "utils.h"
#include "wechat_hook.h"
#include "wechat_service.h"

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

INT64 GetInt64Param(nlohmann::json data, std::string key) {
  INT64 result;
  try {
    result = data[key].get<INT64>();
  } catch (nlohmann::json::exception) {
    result = STR2LL(data[key].get<std::string>());
  }
  return result;
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
  INT64 success = WechatService::GetInstance().GetContacts(vec);
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

std::string CheckLogin(mg_http_message* hm) {
  INT64 success = WechatService::GetInstance().CheckLogin();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}

std::string GetSelfInfo(mg_http_message* hm) {
  common::SelfInfoInner self_info;
  INT64 success = WechatService::GetInstance().GetSelfInfo(self_info);
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  if (success) {
    nlohmann::json j_info = {
        {"name", self_info.name},
        {"city", self_info.city},
        {"province", self_info.province},
        {"country", self_info.country},
        {"account", self_info.account},
        {"wxid", self_info.wxid},
        {"mobile", self_info.mobile},
        {"headImage", self_info.head_img},
        {"signature", self_info.signature},
        {"dataSavePath", self_info.data_save_path},
        {"currentDataPath", self_info.current_data_path},
        {"dbKey", self_info.db_key},
        {"publicKey", self_info.public_key},
        {"privateKey", self_info.private_key},
    };
    ret_data["data"] = j_info;
  }
  return ret_data.dump();
}

std::string GetDBInfo(mg_http_message* hm) {
  std::vector<void*> v_ptr = wxhelper::DB::GetInstance().GetWeChatDbHandles();
  nlohmann::json ret_data = {{"data", nlohmann::json::array()}};
  for (unsigned int i = 0; i < v_ptr.size(); i++) {
    nlohmann::json db_info;
    db_info["tables"] = nlohmann::json::array();
    common::DatabaseInfo* db =
        reinterpret_cast<common::DatabaseInfo*>(v_ptr[i]);
    db_info["handle"] = db->handle;
    std::wstring dbname(db->db_name);
    db_info["databaseName"] = base::utils::WstringToUtf8(dbname);
    for (auto table : db->tables) {
      nlohmann::json table_info = {{"name", table.name},
                                   {"tableName", table.table_name},
                                   {"sql", table.sql},
                                   {"rootpage", table.rootpage}};
      db_info["tables"].push_back(table_info);
    }
    ret_data["data"].push_back(db_info);
  }
  ret_data["code"] = 1;
  ret_data["msg"] = "success";
  return ret_data.dump();
}

std::string ExecSql(mg_http_message* hm) {
  nlohmann::json j_param = nlohmann::json::parse(
      hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  UINT64 db_handle = GetInt64Param(j_param, "dbHandle");
  std::string sql = GetStringParam(j_param, "sql");
  std::vector<std::vector<std::string>> items;
  int success =
      wxhelper::DB::GetInstance().Select(db_handle, sql.c_str(), items);
  nlohmann::json ret_data = {
      {"data", nlohmann::json::array()}, {"code", success}, {"msg", "success"}};
  if (success == 0) {
    ret_data["msg"] = "no data";
    return ret_data.dump();
  }
  for (auto it : items) {
    nlohmann::json temp_arr = nlohmann::json::array();
    for (size_t i = 0; i < it.size(); i++) {
      temp_arr.push_back(it[i]);
    }
    ret_data["data"].push_back(temp_arr);
  }
  return ret_data.dump();
}

std::string LockWeChat(struct mg_http_message* hm) {
  INT64 success = WechatService::GetInstance().LockWeChat();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}

std::string UnLockWeChat(struct mg_http_message* hm) {
  INT64 success = WechatService::GetInstance().UnLockWeChat();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}

}  // namespace wxhelper