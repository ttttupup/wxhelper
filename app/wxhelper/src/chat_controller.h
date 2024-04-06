#ifndef WXHELPER_CHAT_CONTROLLER_H_
#define WXHELPER_CHAT_CONTROLLER_H_
#include "http_controller.h"
#include "spdlog/spdlog.h"
#include <Windows.h>
namespace wxhelper {
class ChatController : public http::HttpController<ChatController> {
   public:
  PATHS_BEGIN
  ADD_PATH("/api/sendTextMsg", SendTextMsg);
  ADD_PATH("/api/sendImagesMsg", SendImageMsg);
  ADD_PATH("/api/sendFileMsg", SendFileMsg);
  ADD_PATH("/api/sendAtText", SendAtText);
  ADD_PATH("/api/sendMultiAtText", SendMultiAtText);
  ADD_PATH("/api/forwardMsg", ForwardMsg);
  PATHS_END

 public:
  static std::string SendTextMsg(std::string, std::string);
  static std::string SendImageMsg(std::string, std::string);
  static std::string SendFileMsg(std::string, std::string);
  static std::string SendAtText(std::string, std::string);
  static std::string SendMultiAtText(std::string, std::string);
  static std::string ForwardMsg(std::string, std::string);
};
}  // namespace wxhelper

#endif