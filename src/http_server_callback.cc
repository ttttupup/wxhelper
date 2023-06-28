#include "pch.h"
#include "http_server_callback.h"
#include "http_server.h"
#include "export.h"
#include "global_context.h"
#include "hooks.h"
namespace common = wxhelper::common;


std::wstring GetWStringParam(nlohmann::json data, std::string key) {
  return wxhelper::Utils::UTF8ToWstring(data[key].get<std::string>());
}

void StartHttpServer(wxhelper::HttpServer *server) {
  int port = server->GetPort();
  std::string lsten_addr = "http://0.0.0.0:" + std::to_string(port);
  if (mg_http_listen(const_cast<mg_mgr *>(server->GetMgr()), lsten_addr.c_str(),
                     EventHandler,
                     const_cast<mg_mgr *>(server->GetMgr())) == NULL) {
    SPDLOG_INFO("http server  listen fail.port:{}", port);
#ifdef _DEBUG
    MG_INFO(("http server  listen fail.port: %d", port));
#endif
    return;
  }
  for (;;) {
    mg_mgr_poll(const_cast<mg_mgr *>(server->GetMgr()), 1000);
  }
}

void EventHandler(struct mg_connection *c, int ev, void *ev_data,
                  void *fn_data) {
  if (ev == MG_EV_OPEN) {
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/websocket")) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/api/*")) {
      HandleHttpRequest(c, hm);
    } else {
      nlohmann::json res = {{"code", 400},
                            {"msg", "invalid url, please check url"},
                            {"data", NULL}};
      std::string ret = res.dump();
      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                    ret.c_str());
    }
  } else if (ev == MG_EV_WS_MSG) {
    HandleWebsocketRequest(c, ev_data);
  }
  (void)fn_data;
}

void HandleHttpRequest(struct mg_connection *c, void *ev_data) {
  struct mg_http_message *hm = (struct mg_http_message *)ev_data;
  std::string ret = R"({"code":200,"msg":"success"})";
  try {
    ret = HttpDispatch(c, hm);
  } catch (nlohmann::json::exception &e) {
    nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
    ret = res.dump();
  }
  if (ret != "") {
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                  ret.c_str());
  }
}

void HandleWebsocketRequest(struct mg_connection *c, void *ev_data) {
  // Got websocket frame. Received data is wm->data. Echo it back!
  struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
  mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
}

std::string HttpDispatch(struct mg_connection *c, struct mg_http_message *hm) {
  std::string ret;
  if (mg_vcasecmp(&hm->method, "GET") == 0) {
    nlohmann::json ret_data = {{"code", 200},
                               {"data", {}},
                               {"msg", "not support get method,use post."}};
    ret = ret_data.dump();
    return ret;
  }

  nlohmann::json j_param = nlohmann::json::parse(
      hm->body.ptr, hm->body.ptr + hm->body.len, nullptr, false);
  if (hm->body.len != 0 && j_param.is_discarded() == true) {
    nlohmann::json ret_data = {
        {"code", 200}, {"data", {}}, {"msg", "json string is invalid."}};
    ret = ret_data.dump();
    return ret;
  }
  if (wxhelper::GlobalContext::GetInstance().state !=
      wxhelper::GlobalContextState::INITIALIZED) {
    nlohmann::json ret_data = {
        {"code", 200}, {"data", {}}, {"msg", "global context is initializing"}};
    ret = ret_data.dump();
    return ret;
  }
  if (mg_http_match_uri(hm, "/api/checkLogin")) {
    INT64 success = wxhelper::GlobalContext::GetInstance().mgr->CheckLogin();
    nlohmann::json ret_data = {
        {"code", success}, {"data", {}}, {"msg", "success"}};
    ret = ret_data.dump();
    return ret;
  } else if (mg_http_match_uri(hm, "/api/userInfo")) {
    common::SelfInfoInner self_info;
    INT64 success = wxhelper::GlobalContext::GetInstance().mgr->GetSelfInfo(self_info);
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
      };
      ret_data["data"] = j_info;
    }
    ret = ret_data.dump();
    return ret;
  } else if (mg_http_match_uri(hm, "/api/sendTextMsg")) {
    std::wstring wxid = GetWStringParam(j_param, "wxid");
    std::wstring msg = GetWStringParam(j_param, "msg");
    INT64 success = wxhelper::GlobalContext::GetInstance().mgr->SendTextMsg(wxid, msg);
    nlohmann::json ret_data = {
        {"code", success}, {"data", {}}, {"msg", "success"}};
    ret = ret_data.dump();
    return ret;
  } else if (mg_http_match_uri(hm, "/api/hookSyncMsg")) {
   INT64 success =  wxhelper::hooks::HookSyncMsg("127.0.0.1",19099,"",3000,false);
   nlohmann::json ret_data = {
       {"code", success}, {"data", {}}, {"msg", "success"}};
   ret = ret_data.dump();
   return ret;
  } else if (mg_http_match_uri(hm, "/api/sendImagesMsg")) {
   std::wstring wxid = GetWStringParam(j_param, "wxid");
   std::wstring path = GetWStringParam(j_param, "imagePath");
   INT64 success = wxhelper::GlobalContext::GetInstance().mgr->SendImageMsg(wxid, path);
   nlohmann::json ret_data = {
       {"code", success}, {"data", {}}, {"msg", "success"}};
   ret = ret_data.dump();
   return ret;
   } else if (mg_http_match_uri(hm, "/api/sendFileMsg")) {
   std::wstring wxid = GetWStringParam(j_param, "wxid");
   std::wstring path = GetWStringParam(j_param, "filePath");
   INT64 success = wxhelper::GlobalContext::GetInstance().mgr->SendFileMsg(wxid, path);
   nlohmann::json ret_data = {
       {"code", success}, {"data", {}}, {"msg", "success"}};
   ret = ret_data.dump();
   return ret;
  } else {
   nlohmann::json ret_data = {
       {"code", 200}, {"data", {}}, {"msg", "not support url"}};
   ret = ret_data.dump();
   return ret;
  }
  nlohmann::json ret_data = {
      {"code", 200}, {"data", {}}, {"msg", "unreachable code."}};
  ret = ret_data.dump();
  return ret;
}