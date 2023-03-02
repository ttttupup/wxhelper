#ifndef SEND_TEXT_H_
#define SEND_TEXT_H_

int SendText(wchar_t* wxid, wchar_t* msg);
int SendAtText(wchar_t* chat_room_id,wchar_t** wxids,int len,wchar_t* msg);
#endif