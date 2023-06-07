#ifndef WXHELPER_SEND_MESSAGE_MGR_H_
#define WXHELPER_SEND_MESSAGE_MGR_H_
#include "base_mgr.h"
namespace wxhelper {
class SendMessageMgr:public BaseMgr {
 public:
   explicit SendMessageMgr(DWORD base);
    ~SendMessageMgr();
   int SendText(wchar_t* wxid, wchar_t* msg);
   int SendAtText(wchar_t* chat_room_id, wchar_t** wxids, int len, wchar_t* msg);
   int SendImage(wchar_t *wxid, wchar_t *image_path);
   int SendFile(wchar_t *wxid, wchar_t *file_path);
   int ForwardMsg(wchar_t *wxid, unsigned long long msgid);
   int ForwardPublicMsg(wchar_t* wxid, wchar_t* title, wchar_t* url, wchar_t* thumburl,wchar_t* senderId,wchar_t* senderName,wchar_t* digest);
   int ForwardPublicMsgByMsgSvrId(wchar_t *wxid, unsigned long long msgid);
   int SendAppletMsg(wchar_t* wxid, wchar_t* appletid);

 private:
};
}  // namespace wxhelper
#endif