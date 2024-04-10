#ifndef WXHELPER_CHAT_CONTROLLER_H_
#define WXHELPER_CHAT_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class ChatController : public http::HttpController<ChatController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/sendTextMsg", SendTextMsg);
  ADD_PATH("/api/sendImagesMsg", SendImageMsg);
  ADD_PATH("/api/sendFileMsg", SendFileMsg);

  ADD_PATH("/api/sendAtText", SendAtText);
  ADD_PATH("/api/sendMultiAtText", SendMultiAtText);
  ADD_PATH("/api/sendCustomEmotion", SendCustomEmotion);
  ADD_PATH("/api/sendApplet", SendApplet);
  ADD_PATH("/api/sendPatMsg", SendPatMsg);

  ADD_PATH("/api/forwardMsg", ForwardMsg);
  ADD_PATH("/api/forwardPublicMsgByMsgId", ForwardPublicMsgByMsgId);
  ADD_PATH("/api/forwardPublicMsg", ForwardPublicMsg);

  PATHS_END

 public:
  /// @brief 发送文本
  /// @param params  json
  /// @return json
  static std::string SendTextMsg(std::string params);
  /// @brief 发送图片
  /// @param params json
  /// @return json
  static std::string SendImageMsg(std::string params);
  /// @brief 发送文件
  /// @param params json
  /// @return json
  static std::string SendFileMsg(std::string params);

  /// @brief 发送@文本
  /// @param params json
  /// @return json
  static std::string SendAtText(std::string params);
  /// @brief 发送多个@文本
  /// @param params json
  /// @return json
  static std::string SendMultiAtText(std::string params);
  /// @brief 发送自定义表情
  /// @param params json
  /// @return json
  static std::string SendCustomEmotion(std::string params);
  /// @brief 发送小程序
  /// @param params json
  /// @return json
  static std::string SendApplet(std::string params);
  /// @brief 拍一拍
  /// @param params json
  /// @return json
  static std::string SendPatMsg(std::string params);


   /// @brief 转发消息
  /// @param params json
  /// @return json
  static std::string ForwardMsg(std::string params);
  /// @brief 转发公众号消息
  /// @param params json
  /// @return json
  static std::string ForwardPublicMsgByMsgId(std::string params);
  /// @brief 发送公众号消息
  /// @param params json
  /// @return json
  static std::string ForwardPublicMsg(std::string params);
  /// @brief 获取联系人昵称
  /// @param params json
  /// @return json
  static std::string GetContactOrChatRoomNickname(std::string params);
};
}  // namespace wxhelper

#endif