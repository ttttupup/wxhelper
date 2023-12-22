#ifndef WXHELPER_HTTP_URL_HANDLER_H_
#define WXHELPER_HTTP_URL_HANDLER_H_
#include <string>

#include "mongoose.h"
namespace wxhelper {
std::string SendTextMsg(struct mg_http_message *hm);
std::string HookSyncMsg(struct mg_http_message *hm);
std::string GetContacts(struct mg_http_message *hm);
std::string UnHookSyncMsg(struct mg_http_message *hm);
std::string CheckLogin(struct mg_http_message *hm);
std::string GetSelfInfo(struct mg_http_message *hm);
std::string GetDBInfo(struct mg_http_message *hm);
std::string ExecSql(struct mg_http_message *hm);
std::string LockWeChat(struct mg_http_message *hm);
std::string UnLockWeChat(struct mg_http_message *hm);
std::string EnterWeChat(struct mg_http_message* hm);
std::string ForwardMsg(struct mg_http_message* hm);
std::string SendImageMsg(struct mg_http_message* hm);
std::string SendFileMsg(struct mg_http_message* hm);
std::string SendAtText(struct mg_http_message* hm);
std::string SendMultiAtText(struct mg_http_message* hm);
std::string GetLoginUrl(struct mg_http_message* hm);
}  // namespace wxhelper

#endif