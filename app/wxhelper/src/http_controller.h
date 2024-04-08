
#ifndef WXHELPER_HTTP_CONTROLLER_H_
#define WXHELPER_HTTP_CONTROLLER_H_

#include "http_router.h"
#include "singleton.h"
namespace http {
#define PATHS_BEGIN static void AddPath() {
#define ADD_PATH(path, method) RegisterPath(path, &method)
#define PATHS_END }

class BaseHttpController {
 public:
  virtual ~BaseHttpController() {}
};

template <typename T>
class HttpController : public base::Singleton<T>{
 public:
  virtual ~HttpController() {}

 protected:
  HttpController() {}

  static void RegisterPath(const std::string &path, HttpHandler function) {
    HttpRouter::GetInstance().AddPathRouting(path, function);
  }

 private:
  struct Registrator {
   public:
    Registrator() { T::AddPath(); }
  };

  static Registrator registrator_;
  virtual void *GetRegistrator() { return &registrator_; }
};

template <typename T>
typename HttpController<T>::Registrator HttpController<T>::registrator_;
}  // namespace http
#endif
