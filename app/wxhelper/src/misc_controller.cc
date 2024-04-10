#include "misc_controller.h"

#include "json_utils.h"
#include "memory.h"
#include "nlohmann/json.hpp"
#include "offset.h"
#include "spdlog/spdlog.h"
#include "tinyxml2.h"
#include "utils.h"
#include "wechat_db.h"
#include "wechat_interface.h"
#include "wxutils.h"

namespace offset = wechat::offset;
namespace prototype = wechat::prototype;
namespace func = wechat::function;
namespace utils = base::utils;
namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {
std::string MiscController::CheckLogin(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t accout_service_addr = base_addr + offset::kGetAccountServiceMgr;
  func::__GetAccountService GetSevice =
      (func::__GetAccountService)accout_service_addr;
  uint64_t service_addr = GetSevice();
  if (service_addr) {
    success = *(uint64_t *)(service_addr + 0x7F8);
  }
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetUserInfo(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t accout_service_addr = base_addr + offset::kGetAccountServiceMgr;
  uint64_t get_app_data_save_path_addr =
      base_addr + offset::kGetAppDataSavePath;
  uint64_t get_current_data_path_addr = base_addr + offset::kGetCurrentDataPath;
  func::__GetAccountService GetSevice =
      (func::__GetAccountService)accout_service_addr;
  func::__GetDataSavePath GetDataSavePath =
      (func::__GetDataSavePath)get_app_data_save_path_addr;
  func::__GetCurrentDataPath GetCurrentDataPath =
      (func::__GetCurrentDataPath)get_current_data_path_addr;
  wechat::SelfInfoInner out;
  uint64_t service_addr = GetSevice();
  if (service_addr) {
    if (*(int64_t *)(service_addr + 0x80) == 0 ||
        *(int64_t *)(service_addr + 0x80 + 0x10) == 0) {
      out.wxid = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x80 + 0x18) == 0xF) {
        out.wxid = std::string((char *)(service_addr + 0x80),
                               *(int64_t *)(service_addr + 0x80 + 0x10));
      } else {
        out.wxid = std::string(*(char **)(service_addr + 0x80),
                               *(int64_t *)(service_addr + 0x80 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x108) == 0 ||
        *(int64_t *)(service_addr + 0x108 + 0x10) == 0) {
      out.account = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x108 + 0x18) == 0xF) {
        out.account = std::string((char *)(service_addr + 0x108),
                                  *(int64_t *)(service_addr + 0x108 + 0x10));
      } else {
        out.account = std::string(*(char **)(service_addr + 0x108),
                                  *(int64_t *)(service_addr + 0x108 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x128) == 0 ||
        *(int64_t *)(service_addr + 0x128 + 0x10) == 0) {
      out.mobile = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x128 + 0x18) == 0xF) {
        out.mobile = std::string((char *)(service_addr + 0x128),
                                 *(int64_t *)(service_addr + 0x128 + 0x10));
      } else {
        out.mobile = std::string(*(char **)(service_addr + 0x128),
                                 *(int64_t *)(service_addr + 0x128 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x148) == 0 ||
        *(int64_t *)(service_addr + 0x148 + 0x10) == 0) {
      out.signature = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x148 + 0x18) == 0xF) {
        out.signature = std::string((char *)(service_addr + 0x148),
                                    *(int64_t *)(service_addr + 0x148 + 0x10));
      } else {
        out.signature = std::string(*(char **)(service_addr + 0x148),
                                    *(int64_t *)(service_addr + 0x148 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x168) == 0 ||
        *(int64_t *)(service_addr + 0x168 + 0x10) == 0) {
      out.country = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x168 + 0x18) == 0xF) {
        out.country = std::string((char *)(service_addr + 0x168),
                                  *(int64_t *)(service_addr + 0x168 + 0x10));
      } else {
        out.country = std::string(*(char **)(service_addr + 0x168),
                                  *(int64_t *)(service_addr + 0x168 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x188) == 0 ||
        *(int64_t *)(service_addr + 0x188 + 0x10) == 0) {
      out.province = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x188 + 0x18) == 0xF) {
        out.province = std::string((char *)(service_addr + 0x188),
                                   *(int64_t *)(service_addr + 0x188 + 0x10));
      } else {
        out.province = std::string(*(char **)(service_addr + 0x188),
                                   *(int64_t *)(service_addr + 0x188 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x1A8) == 0 ||
        *(int64_t *)(service_addr + 0x1A8 + 0x10) == 0) {
      out.city = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x1A8 + 0x18) == 0xF) {
        out.city = std::string((char *)(service_addr + 0x1A8),
                               *(int64_t *)(service_addr + 0x1A8 + 0x10));
      } else {
        out.city = std::string(*(char **)(service_addr + 0x1A8),
                               *(int64_t *)(service_addr + 0x1A8 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x1E8) == 0 ||
        *(int64_t *)(service_addr + 0x1E8 + 0x10) == 0) {
      out.name = std::string();
    } else {
      if (*(int64_t *)(service_addr + 0x1E8 + 0x18) == 0xF) {
        out.name = std::string((char *)(service_addr + 0x1E8),
                               *(int64_t *)(service_addr + 0x1E8 + 0x10));
      } else {
        out.name = std::string(*(char **)(service_addr + 0x1E8),
                               *(int64_t *)(service_addr + 0x1E8 + 0x10));
      }
    }

    if (*(int64_t *)(service_addr + 0x450) == 0 ||
        *(int64_t *)(service_addr + 0x450 + 0x10) == 0) {
      out.head_img = std::string();
    } else {
      out.head_img = std::string(*(char **)(service_addr + 0x450),
                                 *(int64_t *)(service_addr + 0x450 + 0x10));
    }

    if (*(int64_t *)(service_addr + 0x7B8) == 0 ||
        *(int64_t *)(service_addr + 0x7B8 + 0x10) == 0) {
      out.public_key = std::string();
    } else {
      out.public_key = std::string(*(char **)(service_addr + 0x7B8),
                                   *(int64_t *)(service_addr + 0x7B8 + 0x10));
    }

    if (*(int64_t *)(service_addr + 0x7D8) == 0 ||
        *(int64_t *)(service_addr + 0x7D8 + 0x10) == 0) {
      out.private_key = std::string();
    } else {
      out.private_key = std::string(*(char **)(service_addr + 0x7D8),
                                    *(int64_t *)(service_addr + 0x7D8 + 0x10));
    }

    if (*(int64_t *)(service_addr + 0x6E0) == 0 ||
        *(int64_t *)(service_addr + 0x6E8) == 0) {
      out.db_key = std::string();
    } else {
      int64_t byte_addr = *(int64_t *)(service_addr + 0x6E0);
      int64_t len = *(int64_t *)(service_addr + 0x6E8);
      out.db_key =
          base::utils::Bytes2Hex((BYTE *)byte_addr, static_cast<int>(len));
    }

    uint64_t flag = *(uint64_t *)(service_addr + 0x7F8);
    if (flag == 1) {
      prototype::WeChatString current_data_path;
      GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&current_data_path));
      if (current_data_path.ptr) {
        out.current_data_path = base::utils::WstringToUtf8(
            std::wstring(current_data_path.ptr, current_data_path.length));
      } else {
        out.current_data_path = std::string();
      }
    }
  }
  prototype::WeChatString data_save_path;
  GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&data_save_path));
  if (data_save_path.ptr) {
    out.data_save_path = base::utils::WstringToUtf8(
        std::wstring(data_save_path.ptr, data_save_path.length));
  } else {
    out.data_save_path = std::string();
  }
  success = 1;
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  nlohmann::json j_info = {
      {"name", out.name},
      {"city", out.city},
      {"province", out.province},
      {"country", out.country},
      {"account", out.account},
      {"wxid", out.wxid},
      {"mobile", out.mobile},
      {"headImage", out.head_img},
      {"signature", out.signature},
      {"dataSavePath", out.data_save_path},
      {"currentDataPath", out.current_data_path},
      {"dbKey", out.db_key},
      {"publicKey", out.public_key},
      {"privateKey", out.private_key},
  };
  ret["data"] = j_info;
  return ret.dump();
}
std::string MiscController::GetSNSFirstPage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetSNSNextPage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::AddFavFromMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::AddFavFromImage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DecodeImage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetVoiceByMsgId(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DoOcrTask(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::LockWeChat(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t lock_mgr_addr = base_addr + offset::kGetLockWechatMgr;
  uint64_t request_lock_addr = base_addr + offset::kRequestLockWechat;
  func::__GetLockWechatMgr get_lock_mgr =
      (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestLockWechat request_lock =
      (func::__RequestLockWechat)request_lock_addr;
  uint64_t mgr = get_lock_mgr();
  success = request_lock(mgr);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::UnlockWeChat(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t lock_mgr_addr = base_addr + offset::kGetLockWechatMgr;
  uint64_t request_unlock_addr = base_addr + offset::kRequestUnLockWechat;
  func::__GetLockWechatMgr get_lock_mgr =
      (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestUnLockWechat request_unlock =
      (func::__RequestUnLockWechat)request_unlock_addr;
  uint64_t mgr = get_lock_mgr();
  success = request_unlock(mgr);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::ClickEnterWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetLoginUrl(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t login_mgr_addr = base_addr + offset::kGetQRCodeLoginMgr;
  func::__GetQRCodeLoginMgr get = (func::__GetQRCodeLoginMgr)login_mgr_addr;
  uint64_t addr = get();
  std::string login_url = wxutils::ReadWeChatStr(addr + 0x68);
  success = 1;
  nlohmann::json ret = {
      {"code", success},
      {"data", {"loginUrl", "http://weixin.qq.com/x/" + login_url}},
      {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::TranslateVoice(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetTranslateVoiceText(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::OpenUrlByWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::ConfirmReceipt(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DownloadAttach(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper
