#ifndef WXHELPER_HTTP_CLIENT_H_
#define WXHELPER_HTTP_CLIENT_H_
#include <string>

#include "mongoose.h"
#include "noncopyable.h"
#include "singleton.h"

namespace http {

struct Data {
  bool done;
   int timeout;
  std::string url;
  std::string post_data;
};

class HttpClient : public NonCopyable {
 public:
  explicit HttpClient(std::string url, int timeout);
  void SendRequest(const std::string &content);
private:
 static void OnHttpEvent(struct mg_connection *c, int ev, void *ev_data,
                   void *fn_data);

 private:
  std::string url_;
  int timeout_;
};

}  // namespace http
#endif