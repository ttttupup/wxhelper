#ifndef WXHELPER_CHAT_ROOM_MGR_H_
#define WXHELPER_CHAT_ROOM_MGR_H_
#include "wechat_function.h"
#include "base_mgr.h"
namespace wxhelper {
class ChatRoomMgr:public BaseMgr {
 public:
   explicit ChatRoomMgr(DWORD base);
    ~ChatRoomMgr();
   int GetChatRoomDetailInfo(wchar_t* chat_room_id,
                                   ChatRoomInfoInner& room_info);
   int DelMemberFromChatRoom(wchar_t* chat_room_id, wchar_t** wxids,
                                   int len);
   int AddMemberToChatRoom(wchar_t* chat_room_id, wchar_t** wxids,
                                 int len);

   int GetMemberFromChatRoom(wchar_t* chat_room_id, ChatRoomInner& out);
   int ModChatRoomMemberNickName(wchar_t* chat_room_id, wchar_t* wxid,
                                       wchar_t* nick);

   int SetTopMsg(wchar_t* wxid, ULONG64 msg_id);
   int RemoveTopMsg(wchar_t* chat_room_id, ULONG64 msg_id);

   std::wstring GetChatRoomMemberNickname(wchar_t* chat_room_id,
                                                wchar_t* wxid);
};
}  // namespace wxhelper
#endif