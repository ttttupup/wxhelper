#include "contacts_controller.h"

#include "nlohmann/json.hpp"

std::string wxhelper::ContactsController::GetContactList(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
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
