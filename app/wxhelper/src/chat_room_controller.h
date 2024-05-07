#ifndef WXHELPER_CHAT_ROOM_CONTROLLER_H_
#define WXHELPER_CHAT_ROOM_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class ChatRoomController : public http::HttpController<ChatRoomController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/getChatRoomDetailInfo", GetChatRoomDetailInfo);
  ADD_PATH("/api/getMemberFromChatRoom", GetMemberFromChatRoom);

  ADD_PATH("/api/addMemberToChatRoom", AddMemberToChatRoom);
  ADD_PATH("/api/delMemberFromChatRoom", DelMemberFromChatRoom);

  ADD_PATH("/api/createChatRoom", CreateChatRoom);
  ADD_PATH("/api/quitChatRoom", QuitChatRoom);
  ADD_PATH("/api/inviteMemberToChatRoom", InviteMemberToChatRoom);

  ADD_PATH("/api/topMsg", TopMsg);
  ADD_PATH("/api/removeTopMsg", RemoveTopMsg);

  ADD_PATH("/api/getChatRoomMemberNickname", GetChatRoomMemberNickname);
  ADD_PATH("/api/modifyChatRoomMemberNickName", ModifyChatRoomMemberNickName);
  PATHS_END

 public:
  /// @brief 获取群聊详情
  /// @param params  json
  /// @return json
  static std::string GetChatRoomDetailInfo(std::string params);
  /// @brief 获取群成员
  /// @param params json
  /// @return json
  static std::string GetMemberFromChatRoom(std::string params);

  /// @brief 添加群成员
  /// @param params json
  /// @return json
  static std::string AddMemberToChatRoom(std::string params);
  /// @brief 删除群成员
  /// @param params json
  /// @return json
  static std::string DelMemberFromChatRoom(std::string params);
  /// @brief 创建群聊
  /// @param params json
  /// @return json
  static std::string CreateChatRoom(std::string params);
  /// @brief 退出群聊
  /// @param params json
  /// @return json
  static std::string QuitChatRoom(std::string params);
  /// @brief 邀请成员入群
  /// @param params json
  /// @return json
  static std::string InviteMemberToChatRoom(std::string params);

  /// @brief 群聊消息置顶
  /// @param params json
  /// @return json
  static std::string TopMsg(std::string params);
  /// @brief 取消群聊消息置顶
  /// @param params json
  /// @return json
  static std::string RemoveTopMsg(std::string params);

  /// @brief 获取群聊成员昵称
  /// @param params json
  /// @return json
  static std::string GetChatRoomMemberNickname(std::string params);
  /// @brief 修改群聊成员昵称
  /// @param params json
  /// @return json
  static std::string ModifyChatRoomMemberNickName(std::string params);
};
}  // namespace wxhelper

#endif