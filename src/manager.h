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
 private:
  UINT64 base_addr_;
};

}  // namespace wxhelper
#endif