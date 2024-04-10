#include "contacts_controller.h"

#include "nlohmann/json.hpp"
#include "offset.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"
#include "json_utils.h"

namespace offset = wechat::offset;
namespace prototype = wechat::prototype;
namespace func = wechat::function;
namespace utils = base::utils;
namespace jsonutils = wxhelper::jsonutils;

std::string wxhelper::ContactsController::GetContactList(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t get_contact_mgr_addr = base_addr + offset::kGetContactMgr;
  uint64_t get_contact_list_addr = base_addr + offset::kGetContactList;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContactList get_contact_list =
      (func::__GetContactList)get_contact_list_addr;
  uint64_t mgr = get_contact_mgr();
  uint64_t contact_vec[3] = {0, 0, 0};
  success = get_contact_list(mgr, reinterpret_cast<uint64_t>(&contact_vec));

  uint64_t start = contact_vec[0];
  uint64_t end = contact_vec[2];
  std::vector<wechat::ContactInner> vec;

  while (start < end) {
    wechat::ContactInner temp;
    temp.wxid = wxutils::ReadWstringThenConvert(start + 0x10);
    temp.custom_account = wxutils::ReadWstringThenConvert(start + 0x30);
    temp.encrypt_name = wxutils::ReadWstringThenConvert(start + 0x50);
    temp.remark = wxutils::ReadWstringThenConvert(start + 0x80);
    temp.remark_pinyin = wxutils::ReadWstringThenConvert(start + 0x148);
    temp.remark_pinyin_all = wxutils::ReadWstringThenConvert(start + 0x168);
    temp.label_ids = wxutils::ReadWstringThenConvert(start + 0xc0);
    temp.nickname = wxutils::ReadWstringThenConvert(start + 0xA0);
    temp.pinyin = wxutils::ReadWstringThenConvert(start + 0x108);
    temp.pinyin_all = wxutils::ReadWstringThenConvert(start + 0x128);
    temp.verify_flag = *(int32_t *)(start + 0x70);
    temp.type = *(int32_t *)(start + 0x74);
    temp.reserved1 = *(int32_t *)(start + 0x1F0);
    temp.reserved2 = *(int32_t *)(start + 0x1F4);
    vec.push_back(temp);
    start += 0x6A8;
  }
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
