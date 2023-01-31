#ifndef CHAT_ROOM_H_
#define CHAT_ROOM_H_
#include "wechat_data.h"

int GetChatRoomDetailInfo(wchar_t* chat_room_id, ChatRoomInfoInner& room_info);
int DelMemberFromChatRoom(wchar_t* chat_room_id,wchar_t** wxids,int len);
int AddMemberToChatRoom(wchar_t* chat_room_id, wchar_t** wxids,int len);

int GetMemberFromChatRoom(wchar_t* chat_room_id,ChatRoomInner & out);
int ModChatRoomMemberNickName(wchar_t* chat_room_id,wchar_t* wxid,wchar_t * nick);
#endif