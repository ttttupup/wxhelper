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

 private:
};
}  // namespace wxhelper
#endif