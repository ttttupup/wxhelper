#include "pch.h"
#include "http_handler.h"

#include <nlohmann/json.hpp>

#include "utils.h"
#include "account_mgr.h"
#include "api_route.h"
#include "chat_room_mgr.h"
#include "contact_mgr.h"
#include "db.h"
#include "easylogging++.h"
#include "hooks.h"
#include "misc_mgr.h"
#include "send_message_mgr.h"
#include "sns_mgr.h"
#include "global_context.h"
#include "hooks.h"

using namespace std;
using namespace nlohmann;

namespace wxhelper {
string GetParamOfGetReq(mg_http_message *hm, string name) {
  string ret;
  char *buffer = new char[hm->query.len + 1];
  ZeroMemory(buffer, hm->query.len + 1);
  int len = mg_http_get_var(&hm->query, name.c_str(), buffer, hm->query.len);
  if (len > 0) ret = string(buffer, len);
  delete[] buffer;
  buffer = NULL;
  return ret;
}

int GetIntParam(json data, string key) {
  int result;
  try {
    result = data[key].get<int>();
  } catch (json::exception) {
    result = STRING2INT(data[key].get<string>());
  }
  return result;
}

wstring GetWStringParam(json data, string key) {
  return Utils::UTF8ToWstring(data[key].get<string>());
}

unsigned long long GetULong64Param(json j_data, string key) {
  unsigned long long result = 0;
  try {
    result = j_data[key].get<ULONG64>();
  } catch (json::exception) {
    string value = j_data[key].get<string>();
    istringstream is(value);
    is >> result;
  }
  return result;
}

static vector<wstring> getArrayParam(json j_data, string key) {
  vector<wstring> result;
  wstring param = GetWStringParam(j_data, key);
  result = Utils::split(param, L',');
  return result;
}

string Dispatch(struct mg_connection *c, struct mg_http_message *hm) {
  int is_post = 0;
  string ret;
  if (mg_vcasecmp(&hm->method, "POST") == 0) {
    is_post = 1;
  }
  el::Logger *defaultLogger = el::Loggers::getLogger("default");
  defaultLogger->info("method: %v   body: %v", hm->method.ptr, hm->body.ptr);
  LOG_IF(is_post != 1, INFO) << "request method is not post";

  if (is_post == 0) {
    json ret_data = {{"result", "ERROR"}, {"msg", "not support method"}};
    ret = ret_data.dump();
    return ret;
  }

  json j_param =
      json::parse(hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  if (hm->body.len != 0 && j_param.is_discarded() == true) {
    json ret_data = {{"result", "ERROR"}, {"msg", "json string is invalid."}};
    ret = ret_data.dump();
    return ret;
  }
  int api_number = STRING2INT(GetParamOfGetReq(hm, "type"));
  GlobalContext& g_context = GlobalContext::GetInstance();
  switch (api_number) {
    case WECHAT_IS_LOGIN: {
      int success = -1;
      success = g_context.account_mgr->CheckLogin();
      json ret_data = {{"result", "OK"}, {"code", success}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_SELF_INFO: {
      SelfInfoInner self_info;
      int success = g_context.account_mgr->GetSelfInfo(self_info);
      json ret_data = {{"result", "OK"}, {"code", success}};
      if (success) {
        json j_info = {
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
        };
        ret_data["data"] = j_info;
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_TEXT: {
      wstring wxid = GetWStringParam(j_param, "wxid");
      wstring msg = GetWStringParam(j_param, "msg");
      int success = g_context.send_mgr->SendText(WS2LPWS(wxid), WS2LPWS(msg));
      json ret_data = {{"result", "OK"}, {"code", success}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_AT: {
      break;
      wstring chat_room_id = GetWStringParam(j_param, "chatRoomId");
      vector<wstring> wxids = getArrayParam(j_param, "wxids");
      wstring msg = GetWStringParam(j_param, "msg");
      vector<wchar_t *> wxid_list;
      for (unsigned int i = 0; i < wxids.size(); i++) {
        wxid_list.push_back(WS2LPWS(wxids[i]));
      }
      int success = g_context.send_mgr->SendAtText(WS2LPWS(chat_room_id), wxid_list.data(),
                               wxid_list.size(), WS2LPWS(msg));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_CARD: {
      break;
    }
    case WECHAT_MSG_SEND_IMAGE: {
      wstring receiver = GetWStringParam(j_param, "wxid");
      wstring img_path = GetWStringParam(j_param, "imagePath");
      int success =
          g_context.send_mgr->SendImage(WS2LPWS(receiver), WS2LPWS(img_path));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_FILE: {
      wstring receiver = GetWStringParam(j_param, "wxid");
      wstring file_path = GetWStringParam(j_param, "filePath");
      int success =
          g_context.send_mgr->SendFile(WS2LPWS(receiver), WS2LPWS(file_path));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_ARTICLE: {
      break;
    }
    case WECHAT_MSG_SEND_APP: {
      break;
    }
    case WECHAT_MSG_START_HOOK: {
      int port = GetIntParam(j_param, "port");
      wstring ip = GetWStringParam(j_param, "ip");
      string client_ip = Utils::WstringToUTF8(ip);
      char ip_cstr[16];
      strcpy_s(ip_cstr, client_ip.c_str());
      int success = hooks::HookRecvMsg(ip_cstr, port);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_STOP_HOOK: {
      int success = hooks::UnHookRecvMsg();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_START_IMAGE_HOOK: {
      break;
    }
    case WECHAT_MSG_STOP_IMAGE_HOOK: {
      break;
    }
    case WECHAT_MSG_START_VOICE_HOOK: {
      break;
    }
    case WECHAT_MSG_STOP_VOICE_HOOK: {
      break;
    }
    case WECHAT_CONTACT_GET_LIST: {
      break;
    }
    case WECHAT_CONTACT_CHECK_STATUS: {
      break;
    }
    case WECHAT_CONTACT_DEL: {
      break;
    }
    case WECHAT_CONTACT_SEARCH_BY_CACHE: {
      break;
    }
    case WECHAT_CONTACT_SEARCH_BY_NET: {
      wstring keyword = GetWStringParam(j_param, "keyword");
      UserInfo *user = nullptr;
      int success = g_context.misc_mgr->SearchContactNetScene(WS2LPWS(keyword), &user);
      json ret_data = {{"code", success}, {"result", "OK"}};
      if (user) {
        json info = {
            {"bigImage",  Utils::WCharToUTF8(user->big_image)},
            {"smallImage",  Utils::WCharToUTF8(user->small_image)},
            {"city",  Utils::WCharToUTF8(user->city)},
            {"nation",  Utils::WCharToUTF8(user->nation)},
            {"nickname",  Utils::WCharToUTF8(user->nickname)},
            {"province",  Utils::WCharToUTF8(user->province)},
            {"sex", user->sex},
            {"signature",  Utils::WCharToUTF8(user->signature)},
            {"v2",  Utils::WCharToUTF8(user->v2)},
            {"v3",  Utils::WCharToUTF8(user->v3)},
        };
        ret_data["userInfo"] = info;
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CONTACT_ADD_BY_WXID: {
      wstring user_id = GetWStringParam(j_param, "wxid");
      wstring msg = GetWStringParam(j_param, "msg");
      int success = g_context.contact_mgr->AddFriendByWxid(WS2LPWS(user_id),WS2LPWS(msg));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CONTACT_ADD_BY_V3: {
      break;
    }
    case WECHAT_CONTACT_ADD_BY_PUBLIC_ID: {
      break;
    }
    case WECHAT_CONTACT_VERIFY_APPLY: {
      wstring v3 = GetWStringParam(j_param, "v3");
      wstring v4 = GetWStringParam(j_param, "v4");
      int success = g_context.contact_mgr->VerifyApply(WS2LPWS(v3),WS2LPWS(v4));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CONTACT_EDIT_REMARK: {
      break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_LIST: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      ChatRoomInner out{0};
      int success = g_context.chat_room_mgr->GetMemberFromChatRoom(WS2LPWS(room_id), out);
      json ret_data = {{"code", success}, {"result", "OK"}};
      if (success) {
        json member_info = {
            {"admin", Utils::WstringToUTF8(out.admin)},
            {"chatRoomId", Utils::WstringToUTF8(out.chat_room)},
            {"members", out.members},
        };
        ret_data["data"] = member_info;
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_NICKNAME: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      wstring member_id = GetWStringParam(j_param, "memberId");

      wstring nickname =g_context.chat_room_mgr->GetChatRoomMemberNickname(
          WS2LPWS(room_id), WS2LPWS(member_id));
      json ret_data = {{"code", 1},
                       {"result", "OK"},
                       {"nickname", Utils::WstringToUTF8(nickname)}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_DEL_MEMBER: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      vector<wstring> wxids = getArrayParam(j_param, "memberIds");
      vector<wchar_t *> wxid_list;
      for (unsigned int i = 0; i < wxids.size(); i++) {
        wxid_list.push_back(WS2LPWS(wxids[i]));
      }
      int success = g_context.chat_room_mgr->DelMemberFromChatRoom(
          WS2LPWS(room_id), wxid_list.data(), wxid_list.size());
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_ADD_MEMBER: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      vector<wstring> wxids = getArrayParam(j_param, "memberIds");
      vector<wchar_t *> wxid_list;
      for (unsigned int i = 0; i < wxids.size(); i++) {
        wxid_list.push_back(WS2LPWS(wxids[i]));
      }
      int success = g_context.chat_room_mgr->AddMemberToChatRoom(
          WS2LPWS(room_id), wxid_list.data(), wxid_list.size());
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_SET_ANNOUNCEMENT: {
      break;
    }
    case WECHAT_CHATROOM_SET_CHATROOM_NAME: {
      break;
    }
    case WECHAT_CHATROOM_SET_SELF_NICKNAME: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      wstring wxid = GetWStringParam(j_param, "wxid");
      wstring nick = GetWStringParam(j_param, "nickName");
      int success = g_context.chat_room_mgr->ModChatRoomMemberNickName(
          WS2LPWS(room_id), WS2LPWS(wxid), WS2LPWS(nick));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_DATABASE_GET_HANDLES: {
      vector<void *> v_ptr = DB::GetInstance().GetDbHandles();
      json ret_data = {{"data", json::array()}, {"result", "OK"}};
      for (unsigned int i = 0; i < v_ptr.size(); i++) {
        json db_info;
        db_info["tables"] = json::array();
        DatabaseInfo *db = reinterpret_cast<DatabaseInfo *>(v_ptr[i]);
        db_info["handle"] = db->handle;
        wstring dbname(db->db_name);
        db_info["databaseName"] = Utils::WstringToUTF8(dbname);
        for (auto table : db->tables) {
          json table_info = {{"name", table.name},
                             {"tableName", table.table_name},
                             {"sql", table.sql},
                             {"rootpage", table.rootpage}};
          db_info["tables"].push_back(table_info);
        }
        ret_data["data"].push_back(db_info);
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_DATABASE_BACKUP: {
      break;
    }
    case WECHAT_DATABASE_QUERY: {
      DWORD db_handle = GetIntParam(j_param, "dbHandle");
      wstring sql = GetWStringParam(j_param, "sql");
      string sql_str = Utils::WstringToUTF8(sql);
      vector<vector<string>> items;
      int success = DB::GetInstance().Select(db_handle, sql_str.c_str(), items);
      json ret_data = {
          {"data", json::array()}, {"code", success}, {"result", "OK"}};
      if (success == 0) {
        ret = ret_data.dump();
        break;
      }
      for (auto it : items) {
        json temp_arr = json::array();
        for (size_t i = 0; i < it.size(); i++) {
          temp_arr.push_back(it[i]);
        }
        ret_data["data"].push_back(temp_arr);
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SET_VERSION: {
      break;
    }
    case WECHAT_LOG_START_HOOK: {
      int success = hooks::HookLog();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_LOG_STOP_HOOK: {
      int success = hooks::UnHookLog();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_BROWSER_OPEN_WITH_URL: {
      break;
    }
    case WECHAT_GET_PUBLIC_MSG: {
      break;
    }
    case WECHAT_MSG_FORWARD_MESSAGE: {
      wstring wxid = GetWStringParam(j_param, "wxid");
      ULONG64 msgid = GetULong64Param(j_param, "msgid");
      int success =g_context.send_mgr->ForwardMsg(WS2LPWS(wxid), msgid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_QRCODE_IMAGE: {
      break;
    }
    case WECHAT_GET_A8KEY: {
      break;
    }
    case WECHAT_MSG_SEND_XML: {
      break;
    }
    case WECHAT_LOGOUT: {
      int success = g_context.account_mgr->Logout();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_TRANSFER: {
      wstring wxid = GetWStringParam(j_param, "wxid");
      wstring transcationid = GetWStringParam(j_param, "transcationId");
      wstring transferid = GetWStringParam(j_param, "transferId");
      BOOL response =g_context.misc_mgr->DoConfirmReceipt(
          WS2LPWS(wxid), WS2LPWS(transcationid), WS2LPWS(transferid));
      json ret_data = {{"msg", response}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_CONTACT_ALL: {
      vector<Contact> vec;
      int success = g_context.contact_mgr->GetAllContact(vec);
      json ret_data = {
          {"data", json::array()}, {"code", success}, {"result", "OK"}};

      for (unsigned int i = 0; i < vec.size(); i++) {
        json item = {
            {"customAccount",
             vec[i].custom_account.length > 0
                 ? vec[i].custom_account.ptr != nullptr
                       ? Utils::WCharToUTF8(vec[i].custom_account.ptr)
                       : string()
                 : string()},
            {"delFlag", vec[i].del_flag},
            {"userName", vec[i].encrypt_name.length > 0
                             ? vec[i].encrypt_name.ptr != nullptr
                                   ? Utils::WCharToUTF8(vec[i].encrypt_name.ptr)
                                   : string()
                             : string()},
            {"type", vec[i].type},
            {"verifyFlag", vec[i].verify_flag},
            {"verifyFlag", vec[i].verify_flag},
            {"wxid", vec[i].wxid.length > 0
                         ? Utils::WCharToUTF8(vec[i].wxid.ptr)
                         : string()},
        };
        ret_data["data"].push_back(item);
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_CHATROOM_INFO: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      ChatRoomInfoInner chat_room_detail{0};
      int success = g_context.chat_room_mgr->GetChatRoomDetailInfo(WS2LPWS(room_id),
                                                       chat_room_detail);
      json ret_data = {{"code", success}, {"result", "OK"}};
      if (!success) {
        break;
      }
      json detail = {
          {"chatRoomId",
           chat_room_detail.chat_room_id.length > 0
               ? Utils::WCharToUTF8(chat_room_detail.chat_room_id.ptr)
               : string()},
          {"notice", chat_room_detail.notice.length > 0
                         ? Utils::WCharToUTF8(chat_room_detail.notice.ptr)
                         : string()},
          {"admin", chat_room_detail.admin.length > 0
                        ? Utils::WCharToUTF8(chat_room_detail.admin.ptr)
                        : string()},
          {"xml", chat_room_detail.xml.length > 0
                      ? Utils::WCharToUTF8(chat_room_detail.xml.ptr)
                      : string()},
      };
      ret_data["data"] = detail;
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_IMG_BY_NAME: {
      wstring image_path = GetWStringParam( j_param, "imagePath");
      wstring save_path = GetWStringParam( j_param, "savePath");
      int success =
      g_context.misc_mgr->GetImgByName(WS2LPWS(image_path),WS2LPWS(save_path)); json
      ret_data = {{"code", success}, {"result", "OK"}}; ret =
      ret_data.dump();
      break;
    }
    case WECHAT_DO_OCR: {
      // wstring image_path = GetWStringParam(j_param, "imagePath");
      // string text("");
      // int success = g_context.misc_mgr->DoOCRTask(WS2LPWS(image_path), text);
      // json ret_data = {{"code", success}, {"result", "OK"}, {"text", text}};
      // ret = ret_data.dump();
      break;
    }
    case WECHAT_SEND_PAT_MSG: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      wstring wxid = GetWStringParam(j_param, "wxid");
      int success = g_context.misc_mgr->SendPatMsg(WS2LPWS(room_id), WS2LPWS(wxid));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SET_TOP_MSG: {
      wstring wxid = GetWStringParam(j_param, "wxid");
      ULONG64 msgid = GetULong64Param(j_param, "msgid");
      int success = g_context.chat_room_mgr->SetTopMsg(WS2LPWS(wxid), msgid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_REMOVE_TOP_MSG: {
      wstring room_id = GetWStringParam(j_param, "chatRoomId");
      ULONG64 msgid = GetULong64Param(j_param, "msgid");
      int success = g_context.chat_room_mgr->RemoveTopMsg(WS2LPWS(room_id), msgid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SNS_GET_FIRST_PAGE: {
      int success = g_context.sns_mgr->GetFirstPage();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SNS_GET_NEXT_PAGE: {
      ULONG64 snsid = GetULong64Param(j_param, "snsId");
      int success = g_context.sns_mgr->GetNextPage(snsid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CONTACT_NAME: {
      wstring pri_id = GetWStringParam(j_param, "id");
      wstring name = g_context.contact_mgr->GetContactOrChatRoomNickname(WS2LPWS(pri_id));
      json ret_data = {
          {"code", 1}, {"result", "OK"}, {"name", Utils::WstringToUTF8(name)}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_ATTACH_DOWNLOAD: {
      ULONG64 msg_id = GetULong64Param(j_param, "msgId");
      int success = g_context.misc_mgr->DoDownloadTask(msg_id);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_VOICE: {
      ULONG64 msg_id = GetULong64Param(j_param, "msgId");
      wstring voice_dir = GetWStringParam(j_param, "voiceDir");
      int success = g_context.misc_mgr->GetVoice(msg_id, WS2LPWS(voice_dir));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    default:
      break;
  }

  return ret;
}

HttpHandler::HttpHandler() {}
HttpHandler::~HttpHandler() {}
void HttpHandler::HandlerRequest(struct mg_connection *c, void *ev_data) {
  struct mg_http_message *hm = (struct mg_http_message *)ev_data;
  string ret = R"({"result":"OK"})";
  if (mg_http_match_uri(hm, "/api/")) {
    try {
      ret = Dispatch(c, hm);
    } catch (json::exception &e) {
      json res = {{"result", "ERROR"}, {"msg", e.what()}};
      ret = res.dump();
    }
    if (ret != "") {
      mg_http_reply(c, 200, "", ret.c_str(), 0, 0);
    }
  } else {
    mg_http_reply(c, 500, NULL, "%s", "Invalid URI");
  }
}

}  // namespace wxhelper