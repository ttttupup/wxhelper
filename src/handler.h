#ifndef WXHELPER_HANDLER_H_
#define WXHELPER_HANDLER_H_
#include <mongoose.h>
namespace wxhelper {
class Handler {
 public:
  virtual void HandlerRequest(struct mg_connection *c, void *ev_data) = 0;
};
}  // namespace wxhelper
#endif