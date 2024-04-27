#include "contacts_controller.h"

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "json_utils.h"
#include "wechat_service.h"
namespace jsonutils = wxhelper::jsonutils;

std::string wxhelper::ContactsController::GetContactList(std::string params) {
  std::vector<wechat::ContactInner> vec;
  int64_t success = wechat::WeChatService::GetInstance().GetContacts(vec);
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  for (unsigned int i = 0; i < vec.size(); i++) {
    nlohmann::json item = {
        {"customAccount", vec[i].custom_account},
        {"encryptName", vec[i].encrypt_name},
        {"type", vec[i].type},
        {"verifyFlag", vec[i].verify_flag},
        {"wxid", vec[i].wxid},
        {"nickname", vec[i].nickname},
        {"pinyin", vec[i].pinyin},
        {"pinyinAll", vec[i].pinyin_all},
        {"reserved1", vec[i].reserved1},
        {"reserved2", vec[i].reserved2},
        {"remark", vec[i].remark},
        {"remarkPinyin", vec[i].remark_pinyin},
        {"remarkPinyinAll", vec[i].remark_pinyin_all},
        {"labelIds", vec[i].label_ids},
    };
    ret_data["data"].push_back(item);
  }
  return ret_data.dump();
}

std::string wxhelper::ContactsController::GetContactProfile(
    std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string wxhelper::ContactsController::DelContact(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string wxhelper::ContactsController::SearchContact(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string wxhelper::ContactsController::AddContact(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string wxhelper::ContactsController::VerifyApply(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
