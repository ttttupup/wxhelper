#ifndef WXHELPER_HTTP_CLIENT_H_
#define WXHELPER_HTTP_CLIENT_H_
#include "mongoose.h"
#include "singleton.h"

namespace wxhelper {
struct Data {
  bool done;
  std::string post_data;
};
class HttpClient : public Singleton<HttpClient> {
 public:
  void SendRequest(std::string content);
  void SetConfig(char* url,uint64_t timeout);                        

  static void OnHttpEvent(struct mg_connection *c, int ev, void *ev_data,
                          void *fn_data);
 private:
   std::string url_;
   uint64_t timeout_;
};

}  // namespace wxhelper
#endif