#ifndef WXHELPER_CONTACTS_CONTROLLER_H_
#define WXHELPER_CONTACTS_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class ContactsController : public http::HttpController<ContactsController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/getContactList", GetContactList);
  ADD_PATH("/api/getContactProfile", GetContactProfile);
  ADD_PATH("/api/delContact", DelContact);
  ADD_PATH("/api/searchContact", SearchContact);
  ADD_PATH("/api/addContact", AddContact);
  ADD_PATH("/api/verifyApply", VerifyApply);
  PATHS_END

 public:
  /// @brief 好友通讯录
  /// @param params json
  /// @return json
  static std::string GetContactList(std::string params);
  /// @brief 联系人信息
  /// @param params json
  /// @return json
  static std::string GetContactProfile(std::string params);
  /// @brief 删除好友
  /// @param params json
  /// @return json
  static std::string DelContact(std::string params);
  /// @brief 搜索联系人
  /// @param params json
  /// @return json
  static std::string SearchContact(std::string params);
  /// @brief 加好友
  /// @param params json
  /// @return json
  static std::string AddContact(std::string params);
  /// @brief 通过好友验证
  /// @param params json
  /// @return json
  static std::string VerifyApply(std::string params);
};
}  // namespace wxhelper

#endif
