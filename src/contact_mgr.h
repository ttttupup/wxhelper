#ifndef WXHELPER_CONTACT_MGR_H_
#define WXHELPER_CONTACT_MGR_H_
#include <string>
#include <vector>

#include "base_mgr.h"
#include "wechat_function.h"
namespace wxhelper {
class ContactMgr : public BaseMgr {
 public:
  explicit ContactMgr(DWORD base);
  ~ContactMgr();
  int GetAllContact(std::vector<Contact>& vec);
  int DelContact(wchar_t* wxid);
  std::wstring GetContactOrChatRoomNickname(wchar_t* id);
  int AddFriendByWxid(wchar_t* wxid,wchar_t* msg);
};
}  // namespace wxhelper

#endif;