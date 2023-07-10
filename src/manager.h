#ifndef WXHELPER_MANAGER_H_
#define WXHELPER_MANAGER_H_
#include "Windows.h"
#include "wechat_function.h"
namespace wxhelper {
class Manager {
 public:
  explicit Manager(UINT64 base);
  ~Manager();
  INT64 CheckLogin();
  INT64 GetSelfInfo(common::SelfInfoInner& out);
  INT64 SendTextMsg(const std::wstring& wxid, const std::wstring& msg);
  INT64 SendImageMsg(const std::wstring& wxid, const std::wstring& image_path);
  INT64 SendFileMsg(const std::wstring& wxid, const std::wstring& file_path);
  INT64 GetContacts(std::vector<common::ContactInner> &vec);
  INT64 GetChatRoomDetailInfo(const std::wstring& room_id,
                              common::ChatRoomInfoInner& room_info);
  INT64 AddMemberToChatRoom(const std::wstring& room_id,
                            const std::vector<std::wstring>& members);

  INT64 ModChatRoomMemberNickName(const std::wstring& room_id,
                                  const std::wstring& wxid,
                                  const std::wstring& nickname);

 private:
  UINT64 base_addr_;
};

}  // namespace wxhelper
#endif