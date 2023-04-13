#include "pch.h"
#include "account_mgr.h"
#include "easylogging++.h"


#include "wechat_function.h"

using namespace std;
namespace wxhelper {
    AccountMgr::AccountMgr(DWORD base):BaseMgr(base){

    }
     AccountMgr::~AccountMgr(){

     }
int AccountMgr::GetSelfInfo(SelfInfoInner &out) {
  DWORD accout_service_addr = base_addr_ + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD get_app_save_addr = base_addr_ + WX_GET_APP_DATA_SAVE_PATH_OFFSET;
  DWORD get_current_data_path_addr = base_addr_ + WX_GET_CURRENT_DATA_PATH_OFFSET;
  DWORD service_addr = NULL;
  __asm {
      PUSHAD 
      CALL       accout_service_addr                          
      MOV        service_addr,EAX
      POPAD
  }
  if (service_addr) {
    if (*(DWORD *)(service_addr + 0x44) == 0 ||
        *(DWORD *)(service_addr + 0x44 + 0x10) == 0) {
      out.wxid = string();
    } else {
      if (*(DWORD *)(service_addr + 0x44 + 0x14) == 0xF) {
        out.wxid = string((char *)(service_addr + 0x44),
                          *(DWORD *)(service_addr + 0x44 + 0x10));
      } else {
        out.wxid = string(*(char **)(service_addr + 0x44),
                          *(DWORD *)(service_addr + 0x44 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xA8) == 0 ||
        *(DWORD *)(service_addr + 0xA8 + 0x10) == 0) {
      out.account = string();
    } else {
      if (*(DWORD *)(service_addr + 0xA8 + 0x14) == 0xF) {
        out.account = string((char *)(service_addr + 0xA8),
                             *(DWORD *)(service_addr + 0xA8 + 0x10));
      } else {
        out.account = string(*(char **)(service_addr + 0xA8),
                             *(DWORD *)(service_addr + 0xA8 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xC0) == 0 ||
        *(DWORD *)(service_addr + 0xC0 + 0x10) == 0) {
      out.mobile = string();
    } else {
      if (*(DWORD *)(service_addr + 0xC0 + 0x14) == 0xF) {
        out.mobile = string((char *)(service_addr + 0xC0),
                            *(DWORD *)(service_addr + 0xC0 + 0x10));
      } else {
        out.mobile = string(*(char **)(service_addr + 0xC0),
                            *(DWORD *)(service_addr + 0xC0 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xD8) == 0 ||
        *(DWORD *)(service_addr + 0xD8 + 0x10) == 0) {
      out.signature = string();
    } else {
      if (*(DWORD *)(service_addr + 0xD8 + 0x14) == 0xF) {
        out.signature = string((char *)(service_addr + 0xD8),
                               *(DWORD *)(service_addr + 0xD8 + 0x10));
      } else {
        out.signature = string(*(char **)(service_addr + 0xD8),
                               *(DWORD *)(service_addr + 0xD8 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xF0) == 0 ||
        *(DWORD *)(service_addr + 0xF0 + 0x10) == 0) {
      out.country = string();
    } else {
      if (*(DWORD *)(service_addr + 0xF0 + 0x14) == 0xF) {
        out.country = string((char *)(service_addr + 0xF0),
                             *(DWORD *)(service_addr + 0xF0 + 0x10));
      } else {
        out.country = string(*(char **)(service_addr + 0xF0),
                             *(DWORD *)(service_addr + 0xF0 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x108) == 0 ||
        *(DWORD *)(service_addr + 0x108 + 0x10) == 0) {
      out.province = string();
    } else {
      if (*(DWORD *)(service_addr + 0x108 + 0x14) == 0xF) {
        out.province = string((char *)(service_addr + 0x108),
                              *(DWORD *)(service_addr + 0x108 + 0x10));
      } else {
        out.province = string(*(char **)(service_addr + 0x108),
                              *(DWORD *)(service_addr + 0x108 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x120) == 0 ||
        *(DWORD *)(service_addr + 0x120 + 0x10) == 0) {
      out.city = string();
    } else {
      if (*(DWORD *)(service_addr + 0x120 + 0x14) == 0xF) {
        out.city = string((char *)(service_addr + 0x120),
                          *(DWORD *)(service_addr + 0x120 + 0x10));
      } else {
        out.city = string(*(char **)(service_addr + 0x120),
                          *(DWORD *)(service_addr + 0x120 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x150) == 0 ||
        *(DWORD *)(service_addr + 0x150 + 0x10) == 0) {
      out.name = string();
    } else {
      if (*(DWORD *)(service_addr + 0x150 + 0x14) == 0xF) {
        out.name = string((char *)(service_addr + 0x150),
                          *(DWORD *)(service_addr + 0x150 + 0x10));
      } else {
        out.name = string(*(char **)(service_addr + 0x150),
                          *(DWORD *)(service_addr + 0x150 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x304) == 0 ||
        *(DWORD *)(service_addr + 0x304 + 0x10) == 0) {
      out.head_img = string();
    } else {
      if (*(DWORD *)(service_addr + 0x304 + 0x14) == 0xF) {
        out.head_img = string((char *)(service_addr + 0x304),
                              *(DWORD *)(service_addr + 0x304 + 0x10));
      } else {
        out.head_img = string(*(char **)(service_addr + 0x304),
                              *(DWORD *)(service_addr + 0x304 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x4CC) == 0 ||
        *(DWORD *)(service_addr + 0x4D0) == 0) {
      out.db_key = string();
    } else {
      DWORD byte_addr = *(DWORD *)(service_addr + 0x4CC);
      DWORD len = *(DWORD *)(service_addr + 0x4D0);
      out.db_key = Utils::Bytes2Hex((BYTE *)byte_addr, len);
    }
  }

  WeChatString data_save_path;
  WeChatString current_data_path;

  __asm {
      PUSHAD
      LEA     ECX,data_save_path
      CALL    get_app_save_addr
      LEA     ECX,current_data_path
      CALL    get_current_data_path_addr
      POPAD
  }

  if (data_save_path.ptr) {
    out.data_save_path = Utils::WstringToUTF8(
        wstring(data_save_path.ptr, data_save_path.length));
  }
  else {
    out.data_save_path = string();
  }

  if (current_data_path.ptr) {
    out.current_data_path = Utils::WstringToUTF8(
        wstring(current_data_path.ptr, current_data_path.length));
  } else {
    out.current_data_path = string();
  }
  return 1;
}

int AccountMgr::CheckLogin() {
  int success = -1;
  DWORD accout_service_addr = base_addr_ + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD service_addr = NULL;
  __asm {
      PUSHAD 
      CALL       accout_service_addr                          
      MOV        service_addr,EAX
      POPAD
  }
  if (service_addr) {
    success = *(DWORD *)(service_addr + 0x4C8);
  }
  return success;
}

int AccountMgr::Logout() {
  int success = -1;
  if (!CheckLogin()) {
    return success;
  }
  DWORD account_service_addr = base_addr_ + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD logout_addr = base_addr_ + WX_LOGOUT_OFFSET;
  __asm {
    PUSHAD
    CALL       account_service_addr                              
    PUSH       0x0
    MOV        ECX,EAX
    CALL       logout_addr 
    MOV        success,EAX                         
    POPAD
  }
  return success;
}

}  // namespace wxhelper