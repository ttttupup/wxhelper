#ifndef WXHELPER_WECHAT_INNER_H_
#define WXHELPER_WECHAT_INNER_H_
#include <string>
#include <vector>
#include <cstdint>
namespace wechat {
  struct TableInfo {
  char *name;
  int64_t name_len;
  char *table_name;
  int64_t table_name_len;
  char *sql;
  int64_t sql_len;
  char *rootpage;
  int64_t rootpage_len;
};

struct DatabaseInfo {
  uint64_t handle = 0;
  wchar_t *db_name = NULL;
  int64_t db_name_len = 0;
  std::vector<TableInfo> tables;
  int64_t count = 0;
  int64_t extrainfo = 0;
};

struct SqlResult {
  char *column_name;
  int64_t column_name_len;
  char *content;
  int64_t content_len;
  bool is_blob;
};

struct InnerMessageStruct {
  char *buffer;
  int64_t length;
  ~InnerMessageStruct() {
    if (this->buffer != NULL) {
      delete[] this->buffer;
      this->buffer = NULL;
    }
  }
};

struct SelfInfoInner {
  std::string name;
  std::string city;
  std::string province;
  std::string country;
  std::string account;
  std::string wxid;
  std::string mobile;
  std::string head_img;
  std::string data_save_path;
  std::string signature;
  std::string current_data_path;
  std::string db_key;
  std::string public_key;
  std::string private_key;
};

struct ContactInner {
  std::string wxid;
  std::string custom_account;
  std::string encrypt_name;
  std::string nickname;
  std::string pinyin;
  std::string pinyin_all;
  std::string remark;
  std::string remark_pinyin;
  std::string remark_pinyin_all;
  std::string label_ids;
  int32_t type;
  int32_t verify_flag;
  int32_t reserved1;
  int32_t reserved2;
  ContactInner() {
    wxid = "";
    custom_account = "";
    encrypt_name = "";
    nickname = "";
    pinyin = "";
    pinyin_all = "";
    remark = "";
    remark_pinyin = "";
    remark_pinyin_all = "";
    label_ids = "";
    type = -1;
    verify_flag = -1;
    reserved1 = -1;
    reserved2 = -1;
  }
};

struct ChatRoomInfoInner {
  std::string chat_room_id;
  std::string notice;
  std::string admin;
  std::string xml;
  ChatRoomInfoInner() {
    chat_room_id = "";
    notice = "";
    admin = "";
    xml = "";
  }
};

struct VectorInner {
#ifdef _DEBUG
  int64_t head;
#endif
  int64_t start;
  int64_t finsh;
  int64_t end;
};

struct ChatRoomMemberInner {
  std::string chat_room_id;
  std::string admin;
  std::string admin_nickname;
  std::string member_nickname;
  std::string member;
  ChatRoomMemberInner()
      : chat_room_id(""),
        admin(""),
        admin_nickname(""),
        member_nickname(""),
        member("") {}
};

struct ContactProfileInner {
  std::string wxid;
  std::string account;
  std::string v3;
  std::string nickname;
  std::string head_image;
  ContactProfileInner()
      : wxid(""), account(""), v3(""), nickname(""), head_image("") {}
};

namespace function {
//hook
 typedef  uint64_t(*__DoAddMsg)(uint64_t, uint64_t, uint64_t);
//hook end

typedef uint64_t (*__GetAccountService)();
typedef uint64_t (*__GetDataSavePath)(uint64_t);
typedef uint64_t (*__GetCurrentDataPath)(uint64_t);
typedef uint64_t (*__GetSendMessageMgr)();
typedef uint64_t (*__SendTextMsg)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
                                uint64_t, uint64_t);
typedef uint64_t (*__FreeChatMsg)(uint64_t);

typedef uint64_t (*__SendImageMsg)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__NewChatMsg)(uint64_t);
typedef uint64_t (*__SendFile)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
                             uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetAppMsgMgr)();
typedef uint64_t (*__OperatorNew)(uint64_t);

typedef uint64_t (*__Free)();
typedef uint64_t (*__GetContactMgr)();
typedef uint64_t (*__GetContactList)(uint64_t, uint64_t);

typedef uint64_t (*__GetChatRoomMgr)();
typedef uint64_t (*__NewChatRoomInfo)(uint64_t);
typedef uint64_t (*__FreeChatRoomInfo)(uint64_t);
typedef uint64_t (*__GetChatRoomDetailInfo)(uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__DoAddMemberToChatRoom)(uint64_t, uint64_t, uint64_t, uint64_t);

typedef uint64_t (*__DoModChatRoomMemberNickName)(uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__DoDelMemberFromChatRoom)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetMemberFromChatRoom)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__NewChatRoom)(uint64_t);
typedef uint64_t (*__FreeChatRoom)(uint64_t);

typedef uint64_t (*__DoTopMsg)(uint64_t, uint64_t);
typedef uint64_t (*__RemoveTopMsg)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__InviteMemberToChatRoom)(uint64_t, uint64_t, uint64_t, uint64_t);

typedef uint64_t (*__CreateChatRoom)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__QuitChatRoom)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__ForwardMsg)(uint64_t, uint64_t, uint64_t, uint64_t);

typedef uint64_t (*__GetSNSFirstPage)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetSNSNextPageScene)(uint64_t, uint64_t);

typedef uint64_t (*__GetSNSDataMgr)();
typedef uint64_t (*__GetSnsTimeLineMgr)();
typedef uint64_t (*__GetMgrByPrefixLocalId)(uint64_t, uint64_t);
typedef uint64_t (*__AddFavFromMsg)(uint64_t, uint64_t);
typedef uint64_t (*__GetChatMgr)();
typedef uint64_t (*__GetFavoriteMgr)();
typedef uint64_t (*__AddFavFromImage)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetContact)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__NewContact)(uint64_t);
typedef uint64_t (*__FreeContact)(uint64_t);
typedef uint64_t (*__NewMMReaderItem)(uint64_t);
typedef uint64_t (*__FreeMMReaderItem)(uint64_t);
typedef uint64_t (*__ForwordPublicMsg)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__NewAppMsgInfo)(uint64_t);
typedef uint64_t (*__FreeAppMsgInfo)(uint64_t);
typedef uint64_t (*__ParseAppMsgXml)(uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetPreDownLoadMgr)();
typedef uint64_t (*__PushAttachTask)(uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__GetCustomSmileyMgr)();
typedef uint64_t (*__SendCustomEmotion)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
                                      uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__JsApiShareAppMessage)(uint64_t);
typedef uint64_t (*__InitJsConfig)(uint64_t, uint64_t);
typedef uint64_t (*__SendApplet)(uint64_t, uint64_t, uint64_t, uint64_t);
typedef uint64_t (*__SendAppletSecond)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
                                     uint64_t);
typedef uint64_t (*__GetAppInfoByWaid)(uint64_t, uint64_t);
typedef uint64_t (*__CopyShareAppMessageRequest)(uint64_t, uint64_t);
typedef uint64_t (*__NewWAUpdatableMsgInfo)(uint64_t);
typedef uint64_t (*__FreeWAUpdatableMsgInfo)(uint64_t);
typedef uint64_t (*__SendPatMsg)(uint64_t, uint64_t);
typedef uint64_t (*__GetOCRManager)();
typedef uint64_t (*__DoOCRTask)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);


typedef uint64_t (*__GetLockWechatMgr)();
typedef uint64_t (*__RequestLockWechat)(uint64_t);
typedef uint64_t (*__RequestUnLockWechat)(uint64_t);
typedef uint64_t (*__OnLoginBtnClick)(uint64_t);
typedef uint64_t (*__GetQRCodeLoginMgr)();
typedef uint64_t (*__UpdateMsg)(uint64_t,uint64_t,uint64_t);
typedef uint64_t (*__GetVoiceMgr)();
typedef uint64_t (*__ChatMsg2NetSceneSendMsg)(uint64_t,uint64_t);
typedef uint64_t (*__TranslateVoice)(uint64_t,uint64_t,uint64_t);
typedef uint64_t (*__ShowWebView)(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);
typedef uint64_t (*__NewWebViewPageConfig)(uint64_t);
typedef uint64_t (*__FreeWebViewPageConfig)(uint64_t);
typedef uint64_t (*__GetWebViewMgr)();
typedef uint64_t (*__SetUrl)(uint64_t,uint64_t,uint64_t);

}  // namespace function

namespace prototype {
struct WeChatString {
  wchar_t *ptr;
  int32_t length;
  int32_t max_length;
  int64_t c_ptr = 0;
  int32_t c_len = 0;
  WeChatString() { WeChatString(NULL); }

  WeChatString(const std::wstring &s) {
    ptr = (wchar_t *)(s.c_str());
    length = static_cast<int32_t>(s.length());
    max_length = static_cast<int32_t>(s.length());
  }
  WeChatString(const wchar_t *pStr) { WeChatString((wchar_t *)pStr); }
  WeChatString(int tmp) {
    ptr = NULL;
    length = 0x0;
    max_length = 0x0;
  }
  WeChatString(wchar_t *pStr) {
    ptr = pStr;
    length = static_cast<int32_t>(wcslen(pStr));
    max_length = static_cast<int32_t>(wcslen(pStr));
  }
  void set_value(const wchar_t *pStr) {
    ptr = (wchar_t *)pStr;
    length = static_cast<int32_t>(wcslen(pStr));
    max_length = static_cast<int32_t>(wcslen(pStr) * 2);
  }
};

struct WeChatStr {
  char *ptr;
  int64_t buf;
  int64_t len;
  int64_t maxlen;

  WeChatStr(const char *p) {
    ptr = (char *)p;
    buf = 0;
    len = strlen(p);
    maxlen = len | 0xF;
  }
  WeChatStr() {
    ptr = NULL;
    buf = 0;
    len = 0;
    maxlen = 0xF;
  }
};

}  // namespace prototype

}  // namespace wechat
#endif