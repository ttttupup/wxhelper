#ifndef WXHELPER_HTTP_HANDLER_H_
#define WXHELPER_HTTP_HANDLER_H_
#include "handler.h"
#include "mongoose.h"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
namespace wxhelper {
class HttpHandler : public Handler {
 public:
  HttpHandler();
  ~HttpHandler();
  void HandlerRequest(struct mg_connection *c, void *ev_data);
};

}  // namespace wxhelper
#endif
