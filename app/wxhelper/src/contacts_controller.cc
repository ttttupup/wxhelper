#include "contacts_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "wechat_service.h"
namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {
std::string ContactsController::GetContactList(std::string params) {
  SPDLOG_INFO("GetContactList params:{}", params);
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

std::string ContactsController::GetContactProfile(
    std::string params) {
  SPDLOG_INFO("GetContactProfile params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  wechat::ContactProfileInner profile;
  int64_t success =
      wechat::WeChatService::GetInstance().GetContactByWxid(wxid, profile);
  nlohmann::json ret = {{"code", success}, {"msg", "success"}, {"data", {}}};
  if (success == 1) {
    nlohmann::json contact_profile = {
        {"account", profile.account},   {"headImage", profile.head_image},
        {"nickname", profile.nickname}, {"v3", profile.v3},
        {"wxid", profile.wxid},
    };
    ret["data"] = contact_profile;
  }

  return ret.dump();
}

std::string ContactsController::DelContact(std::string params) {
  SPDLOG_INFO("DelContact params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  int64_t success = wechat::WeChatService::GetInstance().DelContact(wxid);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ContactsController::SearchContact(std::string params) {
  SPDLOG_INFO("SearchContact params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring keyword = jsonutils::GetWStringParam(jp, "keyword");
  wechat::SearchContactInner contact;
  int64_t success =
      wechat::WeChatService::GetInstance().SearchContact(keyword, contact);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  if (success == 1) {
    nlohmann::json info = {
        {"bigImage", contact.big_image},
        {"smallImage", contact.small_image},
        {"city", contact.city},
        {"nation", contact.nation},
        {"nickname", contact.nickname},
        {"province", contact.province},
        {"sex", contact.sex},
        {"signature", contact.signature},
        {"v2", contact.v2},
        {"v3", contact.v3},
    };
    ret["data"] = info;
  }

  return ret.dump();
}

std::string ContactsController::AddContact(std::string params) {
  SPDLOG_INFO("AddContact params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring msg = jsonutils::GetWStringParam(jp, "msg");
  int64_t success =
      wechat::WeChatService::GetInstance().AddFriendByWxid(wxid, msg);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ContactsController::VerifyApply(std::string params) {
  SPDLOG_INFO("VerifyApply params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring v3 = jsonutils::GetWStringParam(jp, "v3");
  std::wstring v4 = jsonutils::GetWStringParam(jp, "v4");
  int32_t permission = jsonutils::GetIntParam(jp, "permission");
  int64_t success =
      wechat::WeChatService::GetInstance().VerifyApply(v3, v4, permission);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
}