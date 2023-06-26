#ifndef WXHELPER_WECHAT_FUNCTION_H_
#define WXHELPER_WECHAT_FUNCTION_H_

namespace wxhelper {
namespace common {
struct InnerMessageStruct {
  char *buffer;
  int length;
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
};

}  // namespace common
namespace V3_9_5_81 {
namespace function {
typedef UINT64(*__GetAccountService)();
typedef UINT64(*__GetDataSavePath)(UINT64);
typedef UINT64(*__GetCurrentDataPath)(UINT64);
typedef void(*__GetSendMessageMgr)();
typedef UINT64 (*__SendTextMsg)(UINT64, UINT64, UINT64, UINT64, UINT64, UINT64,
                                UINT64, UINT64);
typedef void (*__FreeChatMsg)(UINT64);

}  // namespace function
namespace prototype {

#include <string>
#include <vector>

struct WeChatString {
  wchar_t *ptr;
  DWORD length;
  DWORD max_length;
  INT64 c_ptr = 0;
  DWORD c_len = 0;
  WeChatString() { WeChatString(NULL); }

  WeChatString(const std::wstring &s) {
    ptr = (wchar_t *)(s.c_str());
    length = static_cast<DWORD>(s.length());
    max_length = static_cast<DWORD>(s.length() * 2);
  }
  WeChatString(const wchar_t *pStr) { WeChatString((wchar_t *)pStr); }
  WeChatString(int tmp) {
    ptr = NULL;
    length = 0x0;
    max_length = 0x0;
  }
  WeChatString(wchar_t *pStr) {
    ptr = pStr;
    length = static_cast<DWORD>(wcslen(pStr));
    max_length = static_cast<DWORD>(wcslen(pStr) * 2);
  }
  void set_value(const wchar_t *pStr) {
    ptr = (wchar_t *)pStr;
    length = static_cast<DWORD>(wcslen(pStr));
    max_length = static_cast<DWORD>(wcslen(pStr) * 2);
  }
};

}  // namespace prototype
namespace offset {
const UINT64 kGetAccountServiceMgr = 0x8c1230;
const UINT64 kSyncMsg = 0xc39680;
const UINT64 kSyncMsgNext = 0xc39680;
const UINT64 kGetCurrentDataPath = 0xf5d130;
const UINT64 kGetAppDataSavePath = 0x12d7040;
const UINT64 kGetSendMessageMgr = 0x8c00e0;
const UINT64 kSendTextMsg = 0xfcd8d0;
const UINT64 kFreeChatMsg = 0x8aaa00;

const UINT64 kDoAddMsg = 0x1010d80;

}  // namespace offset
}  // namespace V3_9_5_81

}  // namespace wxhelper

#endif