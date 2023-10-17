#ifndef WXHELPER_HTTP_URL_HANDLER_H_
#define WXHELPER_HTTP_URL_HANDLER_H_
#include <string>

#include "mongoose.h"
namespace wxhelper {
std::string SendTextMsg(struct mg_http_message *hm);
}

#endif