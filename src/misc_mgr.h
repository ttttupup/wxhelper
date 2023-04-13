#ifndef WXHELPER_MISC_MGR_H_
#define WXHELPER_MISC_MGR_H_
#include <string>
#include "Windows.h"
#include "base_mgr.h"
#include "wechat_function.h"
namespace wxhelper {
class MiscMgr :public BaseMgr{
 public:
   MiscMgr(DWORD base);
   ~MiscMgr();
   int SendPatMsg(wchar_t* chat_room_id, wchar_t* wxid);
   int DoOCRTask(wchar_t* img_path, std::string& result);
   int DoConfirmReceipt(wchar_t* wxid, wchar_t* transcationid,
                              wchar_t* transferid);
   int DoDownloadTask(ULONG64 msg_id);

   int GetVoice(ULONG64 msg_id, wchar_t* dir);
   int GetImgByName(wchar_t* file_path,wchar_t* save_dir);
   int SearchContactNetScene(wchar_t *keyword,UserInfo ** user);
};
}  // namespace wxhelper

#endif