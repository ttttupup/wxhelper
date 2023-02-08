#include "pch.h"
#include "api.h"
#include <mongoose.h>

#include <nlohmann/json.hpp>


#include "send_text.h"
#include "common.h"
#include "send_image.h"
#include "send_file.h"
#include "hook_recv_msg.h"
#include "get_db_handle.h"
#include "wechat_data.h"
#include "forward.h"
#include "db_operation.h"
#include "contact.h"
#include "chat_room.h"
#include "self_info.h"
#include "hook_img.h"
#include "ocr.h"
#include "pat.h"
#include "confirm_receipt.h"
#include "sns.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
using namespace nlohmann;

#define STR2INT(str) (is_digit(str) ? stoi(str) : 0)
#define WS2LW(wstr) (LPWSTR) wstr.c_str()

static bool kHttpRuning = false;
static HANDLE kHttpThread = NULL;

bool is_digit(string str) {
  if (str.length() == 0) {
    return false;
  }

  for (auto it : str) {
    if (it < '0' || it > '9') {
      return false;
    }
  }
  return true;
}

string get_var(mg_http_message *hm, string name) {
  string ret;
  char *buffer = new char[hm->query.len + 1];
  ZeroMemory(buffer, hm->query.len + 1);
  int len = mg_http_get_var(&hm->query, name.c_str(), buffer, hm->query.len);
  if (len > 0) ret = string(buffer, len);
  delete[] buffer;
  buffer = NULL;
  return ret;
}
/// @brief 获取request中的请求参数int类型
/// @param hm 消息
/// @param data json数据
/// @param key  key
/// @param method 是否是post，暂时全部用post
/// @return int
static int  get_http_req_param_int(mg_http_message *hm, json data, string key, int method){
    int result;
    switch (method) {
      case 0: {
        result = STR2INT(get_var(hm,key).c_str());
        break;
      }
      case 1: {
        try {
          result = data[key].get<int>();
        } catch (json::exception) {
          result = STR2INT(data[key].get<string>());
        }
        break;
      }
      default:
        break;
    }
    return result;
}

/// @brief 获取request中的请求参数
/// @param hm 消息
/// @param data json数据
/// @param key  key
/// @param method 是否是post，暂时全部用post
/// @return 
static wstring  get_http_req_param(mg_http_message *hm, json data, string key, int method){
    wstring result;
    switch (method) {
      case 0: {
        result = utf8_to_unicode(get_var(hm,key).c_str());
        break;
      }
      case 1: {
        result = utf8_to_unicode(data[key].get<string>().c_str());
        break;
      }
      default:
        break;
    }
    return result;
}

static unsigned long long get_http_param_ulong64(mg_http_message *hm,
                                                 json j_data, string key,
                                                 int method) {
    unsigned long long result = 0;
    switch (method) {
      case 0: {
        string value = get_var(hm, key);
        istringstream is(value);
        is >> result;
        break;
      }
      case 1: {
        try {
          result = j_data[key].get<ULONG64>();
        } catch (json::exception) {
          string value = j_data[key].get<string>();
          istringstream is(value);
          is >> result;
        }
        break;
      }
      default:
        break;
    }
    return result;
}

static int get_http_param_int(mg_http_message *hm, json j_data, string key,
                              int method) {
  int result = 0;
  switch (method) {
    case 0: {
      result = STR2INT(get_var(hm, key));
      break;
    }
    case 1: {
      try {
        result = j_data[key].get<int>();
      } catch (json::exception) {
        result = STR2INT(j_data[key].get<string>());
      }
      break;
    }
    default:
      break;
  }
  return result;
}

static vector<wstring> get_http_param_array(mg_http_message *hm, json j_data,
                                            string key, int method) {
  vector<wstring> result;
  switch (method) {
    case 0: {
      result = split(utf8_to_unicode(get_var(hm, key).c_str()), L',');
      break;
    }
    case 1: {
      result = split(utf8_to_unicode(j_data[key].get<string>().c_str()), L',');
      break;
    }
    default:
      break;
  }
  return result;
}

/// @brief api接口入口解析
/// @param hm mg_http_message
/// @param c connection
/// @param ret json数据
void api_handle(mg_http_message *hm, struct mg_connection *c, string &ret) {
  int is_post = 0;

  if (mg_vcasecmp(&hm->method, "POST") == 0) {
    is_post = 1;
  }
  #ifdef _DEBUG
  printf("method:%s body: %s", hm->method.ptr,hm->body.ptr);
  #endif
  if (is_post == 0){
    json ret_data = {{"result", "ERROR"}, {"msg", "not support method"}};
    ret = ret_data.dump();
    return;
  }
  
  json j_param =
      json::parse(hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  if (hm->body.len != 0 && j_param.is_discarded() == true) {
    json ret_data = {{"result", "ERROR"}, {"msg", "json string is invalid."}};
    ret = ret_data.dump();
    return;
  }
  int api_number = STR2INT(get_var(hm, "type"));
  switch (api_number) {
    case WECHAT_IS_LOGIN: {
      int success = CheckLogin();
      json ret_data = {{"result", "OK"}, {"code", success}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_SELF_INFO: {
      SelfInfoInner self_info;
      int success = GetSelfInfo(self_info);
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
            {"smallImage", self_info.small_img},
            {"bigImage", self_info.big_img},
            {"dataRootPath",self_info.data_root_path},
            {"dataSavePath",self_info.data_save_path},
            {"currentDataPath",self_info.current_data_path},
        };
        ret_data["data"] = j_info;
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_TEXT: {
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      wstring msg = get_http_req_param(hm, j_param, "msg", is_post);
      int success = SendText(WS2LW(wxid), WS2LW(msg));
      json ret_data = {{"result", "OK"}, {"code", success}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_AT: {
      break;
    }
    case WECHAT_MSG_SEND_CARD: {
      break;
    }
    case WECHAT_MSG_SEND_IMAGE: {
      wstring receiver = get_http_req_param(hm, j_param, "wxid", is_post);
      wstring img_path = get_http_req_param(hm, j_param, "imagePath", is_post);
      int success = SendImage(WS2LW(receiver), WS2LW(img_path));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_SEND_FILE: {
      wstring receiver = get_http_req_param(hm, j_param, "wxid", is_post);
      wstring file_path = get_http_req_param(hm, j_param, "filePath", is_post);
      int success = SendFile(WS2LW(receiver), WS2LW(file_path));
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
      int port = get_http_req_param_int(hm, j_param, "port", is_post);
      wstring ip = get_http_req_param(hm, j_param, "ip", is_post);
      string client_ip = Wstring2String(ip);
      char ip_cstr[16];
      strcpy_s(ip_cstr,client_ip.c_str());
      int success = HookRecvMsg(ip_cstr,port);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_STOP_HOOK: {
      int success = UnHookRecvMsg();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_START_IMAGE_HOOK: {
      wstring img_dir = get_http_req_param(hm, j_param, "imgDir", is_post);
      int success = HookImg(img_dir);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_MSG_STOP_IMAGE_HOOK: {
      int success = UnHookImg();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
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
      wstring user_id = get_http_req_param(hm, j_param, "wxid", is_post);
      int success = DelContact(WS2LW(user_id));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CONTACT_SEARCH_BY_CACHE: {
      break;
    }
    case WECHAT_CONTACT_SEARCH_BY_NET: {
      break;
    }
    case WECHAT_CONTACT_ADD_BY_WXID: {
      break;
    }
    case WECHAT_CONTACT_ADD_BY_V3: {
      break;
    }
    case WECHAT_CONTACT_ADD_BY_PUBLIC_ID: {
      break;
    }
    case WECHAT_CONTACT_VERIFY_APPLY: {
      break;
    }
    case WECHAT_CONTACT_EDIT_REMARK: {
      break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_LIST: {
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      ChatRoomInner out{0};
      int success = GetMemberFromChatRoom(WS2LW(room_id),out);
      json ret_data = {{"code", success}, {"result", "OK"}};
      if (success){
       json member_info ={
        {"admin",unicode_to_utf8(out.admin)},
        {"chatRoomId",unicode_to_utf8(out.chat_room)},
        {"members",out.members},
       };
       ret_data["data"] = member_info;
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_GET_MEMBER_NICKNAME: {
      break;
    }
    case WECHAT_CHATROOM_DEL_MEMBER: {
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      vector<wstring> wxids = get_http_param_array(hm, j_param, "memberIds", is_post);
      vector<wchar_t *> wxid_list;
      for (unsigned int i = 0; i < wxids.size(); i++){
        wxid_list.push_back(WS2LW(wxids[i]));
      }
      int success = DelMemberFromChatRoom(WS2LW(room_id), wxid_list.data(),wxid_list.size());
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_CHATROOM_ADD_MEMBER: {
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      vector<wstring> wxids = get_http_param_array(hm, j_param, "memberIds", is_post);
      vector<wchar_t *> wxid_list;
      for (unsigned int i = 0; i < wxids.size(); i++){
        wxid_list.push_back(WS2LW(wxids[i]));
      }
      int success = AddMemberToChatRoom(WS2LW(room_id), wxid_list.data(),wxid_list.size());
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
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      wstring nick = get_http_req_param(hm, j_param, "nickName", is_post);
      int success = ModChatRoomMemberNickName(WS2LW(room_id),WS2LW(wxid),WS2LW(nick));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_DATABASE_GET_HANDLES: {
      vector<void *> v_ptr = GetDbHandles();
      json ret_data = {{"data", json::array()}, {"result", "OK"}};
      for (unsigned int i = 0; i < v_ptr.size(); i++) {
          json db_info;
          db_info["tables"] = json::array();
          DatabaseInfo *db = reinterpret_cast<DatabaseInfo *>(v_ptr[i]);
          db_info["handle"] = db->handle;
          db_info["databaseName"] = unicode_to_utf8(db->db_name);
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
      DWORD db_handle = get_http_param_int(hm, j_param, "dbHandle", is_post);
      wstring sql = get_http_req_param(hm, j_param, "sql", is_post);
      string sql_str = unicode_to_utf8(WS2LW(sql));
      vector<vector<string>> items;
      int success = Select(db_handle, sql_str.c_str(),items);
      json ret_data = {{"data", json::array()}, {"code",success},{"result", "OK"}};
      if(success == 0){
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
      break;
    }
    case WECHAT_LOG_STOP_HOOK: {
      break;
    }
    case WECHAT_BROWSER_OPEN_WITH_URL: {
      break;
    }
    case WECHAT_GET_PUBLIC_MSG: {
      break;
    }
    case WECHAT_MSG_FORWARD_MESSAGE: {
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      ULONG64 msgid = get_http_param_ulong64(hm, j_param, "msgid", is_post);
      int success = ForwardMsg(WS2LW(wxid), msgid);
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
      int success = Logout();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_TRANSFER: {
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      wstring transcationid = get_http_req_param(hm, j_param, "transcationId", is_post);
      wstring transferid = get_http_req_param(hm, j_param, "transferId", is_post);
      BOOL response = DoConfirmReceipt(WS2LW(wxid), WS2LW(transcationid), WS2LW(transferid));
      json ret_data = {{"msg", response}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_CONTACT_ALL: {
      vector<Contact> vec;
      int success = GetAllContact(vec);
      json ret_data = {
          {"data", json::array()}, {"code", success}, {"result", "OK"}};

      for (unsigned int i = 0; i < vec.size(); i++) {
        #ifdef _DEBUG
        cout << "vector :" <<i << endl;
        cout<< "custom :" <<  vec[i].custom_account.ptr << endl;
        cout<< "userName :" << vec[i].encrypt_name.ptr << endl;
        cout<< "wxid :" << vec[i].wxid.ptr << endl;
        #endif
          json item = {
              {"customAccount",
               vec[i].custom_account.length > 0
                   ? vec[i].custom_account.ptr != nullptr
                         ? unicode_to_utf8(vec[i].custom_account.ptr)
                         : string()
                   : string()},
              {"delFlag", vec[i].del_flag},
              {"userName", vec[i].encrypt_name.length > 0
                               ? vec[i].encrypt_name.ptr != nullptr
                                     ? unicode_to_utf8(vec[i].encrypt_name.ptr)
                                     : string()
                               : string()},
              {"type", vec[i].type},
              {"verifyFlag", vec[i].verify_flag},
              {"verifyFlag", vec[i].verify_flag},
              {"wxid", vec[i].wxid.length > 0 ? unicode_to_utf8(vec[i].wxid.ptr)
                                              : string()},
          };
          ret_data["data"].push_back(item);
      }
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_CHATROOM_INFO: {
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      ChatRoomInfoInner chat_room_detail{0};
      int success = GetChatRoomDetailInfo(WS2LW(room_id), chat_room_detail);
      json ret_data = {{"code", success}, {"result", "OK"}};
      if(!success){
        break;
      }
      json detail = {
          {"chatRoomId",
           chat_room_detail.chat_room_id.length > 0
               ? unicode_to_utf8(chat_room_detail.chat_room_id.ptr)
               : string()},
          {"notice", chat_room_detail.notice.length > 0
                         ? unicode_to_utf8(chat_room_detail.notice.ptr)
                         : string()},
          {"admin", chat_room_detail.admin.length > 0
                        ? unicode_to_utf8(chat_room_detail.admin.ptr)
                        : string()},
          {"xml", chat_room_detail.xml.length > 0
                      ? unicode_to_utf8(chat_room_detail.xml.ptr)
                      : string()},
      };
      ret_data["data"]=detail;
      ret = ret_data.dump();
      break;
    }
    case WECHAT_GET_IMG_BY_NAME: {
      wstring image_path = get_http_req_param(hm, j_param, "imagePath", is_post);
      wstring save_path = get_http_req_param(hm, j_param, "savePath", is_post);
      int success = GetImgByName(WS2LW(image_path),WS2LW(save_path));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case  WECHAT_DO_OCR:{
      wstring image_path = get_http_req_param(hm, j_param, "imagePath", is_post);
      string text("");
      int success = DoOCRTask(WS2LW(image_path),text);
      json ret_data = {{"code", success}, {"result", "OK"},{"text",text}};
      ret = ret_data.dump();
      break;
    }
    case  WECHAT_SEND_PAT_MSG:{
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      int success = SendPatMsg(WS2LW(room_id),WS2LW(wxid));
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SET_TOP_MSG:{
      wstring wxid = get_http_req_param(hm, j_param, "wxid", is_post);
      ULONG64 msgid = get_http_param_ulong64(hm, j_param, "msgid", is_post);
      int success = SetTopMsg(WS2LW(wxid),msgid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_REMOVE_TOP_MSG:{
      wstring room_id = get_http_req_param(hm, j_param, "chatRoomId", is_post);
      ULONG64 msgid = get_http_param_ulong64(hm, j_param, "msgid", is_post);
      int success = RemoveTopMsg(WS2LW(room_id),msgid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    case WECHAT_SNS_GET_FIRST_PAGE:{
      int success = GetFirstPage();
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
     case WECHAT_SNS_GET_NEXT_PAGE:{
      ULONG64 snsid = get_http_param_ulong64(hm, j_param, "snsId", is_post);
      int success = GetNextPage(snsid);
      json ret_data = {{"code", success}, {"result", "OK"}};
      ret = ret_data.dump();
      break;
    }
    default:
      break;
  }
}

/// @brief 事件回调函数
/// @param c 链接
/// @param ev 事件
/// @param ev_data 事件数据
/// @param fn_data 回调数据
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {0};
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    string ret = R"({"result":"OK"})";
    if (mg_http_match_uri(hm, "/api/")) {
      try {
        api_handle(hm, c, ret);
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
  (void)fn_data;
}
/// @brief http server
/// @param port  端口
void http_server(int port) {
  string lsten_addr = "http://0.0.0.0:" + to_string(port);
  struct mg_mgr mgr;
  // Init manager
  mg_mgr_init(&mgr);
  // Setup listener
  mg_http_listen(&mgr, lsten_addr.c_str(), fn, &mgr);
  // Event loop
  for (;;) mg_mgr_poll(&mgr, 1000);
  // Cleanup
  mg_mgr_free(&mgr);
}

/// @brief 启动http服务
/// @param port 端口
/// @return 成功返回0
int http_start(int port) {
  if (kHttpRuning) {
    return 1;
  }
  #ifdef _DEBUG
    CreateConsole();
  #endif
  kHttpRuning = true;
  kHttpThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)http_server,
                             (LPVOID)port, NULL, 0);
  return 0;
}