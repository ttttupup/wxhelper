#ifndef WXHELPER_WECHAT_SERVICE_H_
#define WXHELPER_WECHAT_SERVICE_H_
#include <windows.h>

#include <string>
#include <vector>

#include "singleton.h"
#include "wechat_interface.h"
namespace wechat {

class WeChatService : public base::Singleton<WeChatService> {
  friend class base::Singleton<WeChatService>;
  ~WeChatService();

 public:
  void Init();
  void SetBaseAddr(uint64_t addr);
  void SetJsApiAddr(uint64_t addr);
  int64_t CheckLogin();
  int64_t GetSelfInfo(SelfInfoInner& out);
  int64_t SendTextMsg(const std::wstring& wxid, const std::wstring& msg);
  int64_t SendImageMsg(const std::wstring& wxid,
                       const std::wstring& image_path);
  int64_t SendFileMsg(const std::wstring& wxid, const std::wstring& file_path);
  int64_t GetContacts(std::vector<ContactInner>& vec);
  int64_t GetChatRoomDetailInfo(const std::wstring& room_id,
                                ChatRoomInfoInner& room_info);
  int64_t AddMemberToChatRoom(const std::wstring& room_id,
                              const std::vector<std::wstring>& members);

  int64_t ModChatRoomMemberNickName(const std::wstring& room_id,
                                    const std::wstring& wxid,
                                    const std::wstring& nickname);
  int64_t DelMemberFromChatRoom(const std::wstring& room_id,
                                const std::vector<std::wstring>& members);
  int64_t GetMemberFromChatRoom(const std::wstring& room_id,
                                ChatRoomMemberInner& member);
  int64_t SetTopMsg(uint64_t msg_id);
  int64_t RemoveTopMsg(const std::wstring& room_id, ULONG64 msg_id);
  int64_t InviteMemberToChatRoom(const std::wstring& room_id,
                                 const std::vector<std::wstring>& wxids);
  int64_t CreateChatRoom(const std::vector<std::wstring>& wxids);
  int64_t QuitChatRoom(const std::wstring& room_id);
  int64_t ForwardMsg(uint64_t msg_id, const std::wstring& wxid);
  int64_t GetSNSFirstPage();
  int64_t GetSNSNextPage(uint64_t sns_id);
  int64_t AddFavFromMsg(uint64_t msg_id);
  int64_t AddFavFromImage(const std::wstring& wxid,
                          const std::wstring& image_path);
  int64_t SendAtText(const std::wstring& room_id,
                     const std::vector<std::wstring>& wxids,
                     const std::wstring& msg);
  std::wstring GetContactOrChatRoomNickname(const std::wstring& wxid);
  int64_t GetContactByWxid(const std::wstring& wxid,
                           ContactProfileInner& profile);
  int64_t DoDownloadTask(uint64_t msg_id);
  int64_t ForwardPublicMsg(const std::wstring& wxid, const std::wstring& title,
                           const std::wstring& url,
                           const std::wstring& thumb_url,
                           const std::wstring& sender_id,
                           const std::wstring& sender_name,
                           const std::wstring& digest);
  int64_t ForwardPublicMsgByMsgId(const std::wstring& wxid, uint64_t msg_id);

  int64_t DecodeImage(const std::wstring& file_path,
                      const std::wstring& save_dir);
  int64_t GetVoiceByDB(ULONG64 msg_id, const std::wstring& dir);
  int64_t SendCustomEmotion(const std::wstring& file_path,
                            const std::wstring& wxid);
  int64_t SendApplet(const std::wstring& recv_wxid,
                     const std::wstring& waid_suff, const std::wstring& waid_w,
                     const std::string& waid_s, const std::string& wa_wxid,
                     const std::string& json_param,
                     const std::string& head_image,
                     const std::string& big_image,
                     const std::string& index_page);
  int64_t SendPatMsg(const std::wstring& room_id, const std::wstring& wxid);
  int64_t DoOCRTask(const std::wstring& img_path, std::string& result);
  int64_t LockWeChat();
  int64_t UnlockWeChat();
  int64_t EnterWeChat();
  int64_t SendMultiAtText(
      const std::wstring& room_id,
      const std::vector<std::pair<std::wstring, std::wstring>>& at);
  std::string GetLoginUrl();

  int64_t TranslateVoice(uint64_t msg_id);
  std::string GetTranslateVoiceText(uint64_t msg_id);
  int64_t OpenUrlByWeChatBrowser(const std::wstring& url, int flag);

 private:
  uint64_t base_addr_;
  uint64_t js_api_addr_;
};

}  // namespace wechat

#endif