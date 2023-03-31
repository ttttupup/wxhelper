#ifndef WXHELPER_WXHELPER_H_
#define WXHELPER_WXHELPER_H_
#include "http_server.h"
namespace wxhelper {

class WXHelper {
 public:
 static WXHelper &GetInstance();
  
  int http_start(int port);
  int http_close();

 private:
  WXHelper(){};
  WXHelper(const WXHelper &) = delete; 
  WXHelper &operator=(const WXHelper &) = delete; 
  ~WXHelper(){};
};
}  // namespace wxhelper

#endif