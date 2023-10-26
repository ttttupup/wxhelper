#ifndef WXHELPER_HTTP_CLIENT_H_
#define WXHELPER_HTTP_CLIENT_H_
#include <string>

#include "mongoose.h"
#include "singleton.h"

namespace http {
static std::string kUrl = "http://127.0.0.1:8000";
static uint64_t kTimeout = 3000;
struct Data {
  bool done;
  std::string post_data;
};

class HttpClient {
 public:
  static void SendRequest(const std::string &content);
  static void SetConfig(std::string url, uint64_t timeout);
  static void OnHttpEvent(struct mg_connection *c, int ev, void *ev_data,
                          void *fn_data);

};

}  // namespace http
#endif