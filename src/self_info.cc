#include "pch.h"
#include "self_info.h"

#include "common.h"

#include "wechat_data.h"
using namespace std;

#define WX_SELF_NAME_OFFSET 0x2E2CE48
#define WX_SELF_MOBILE_OFFSET 0x2E2CDB8
#define WX_SELF_CITY_OFFSET 0x2E2CE18
#define WX_SELF_PROVINCE_OFFSET 0x2E2CE00
#define WX_SELF_COUNTRY_OFFSET 0x2E2CDE8
#define WX_SELF_ACCOUNT_OFFSET 0x2e2d1d0
#define WX_SELF_ID_OFFSET 0x2E2CD3C
#define WX_SELF_SMALL_IMG_OFFSET 0x2E2D014
#define WX_SELF_BIG_IMG_OFFSET 0x2E2CFFC
#define WX_LOGIN_STATUS_OFFSET 0x2E2D1C0
#define WX_APP_DATA_ROOT_PATH_OFFSET 0x2E73010
#define WX_APP_DATA_SAVE_PATH_OFFSET 0x2E52DB0
#define WX_CURRENT_DATA_PATH_OFFSET 0x2E4F290

#define WX_LOGOUT_OFFSET  0xdd5c90
#define WX_ACCOUNT_SERVICE_OFFSET 0x707960
#define WX_GET_APP_DATA_SAVE_PATH_OFFSET 0xeb4b00
#define WX_GET_CURRENT_DATA_PATH_OFFSET   0xc11140
int GetSelfInfo(SelfInfoInner &out) {
  DWORD base = GetWeChatWinBase();
  DWORD accout_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD get_app_save_addr = base + WX_GET_APP_DATA_SAVE_PATH_OFFSET;
  DWORD get_current_data_path_addr = base + WX_GET_CURRENT_DATA_PATH_OFFSET;
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
      out.wxid = string(*(char **)(service_addr + 0x44),
                        *(DWORD *)(service_addr + 0x44 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0xA8) == 0 ||
        *(DWORD *)(service_addr + 0xA8 + 0x10) == 0) {
      out.account = string();
    } else {
      out.account = string(*(char **)(service_addr + 0xA8),
                           *(DWORD *)(service_addr + 0xA8 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0xC0) == 0 ||
        *(DWORD *)(service_addr + 0xC0 + 0x10) == 0) {
      out.mobile = string();
    } else {
      out.mobile = string((char *)(service_addr + 0xC0),
                          *(DWORD *)(service_addr + 0xC0 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0xD8) == 0 ||
        *(DWORD *)(service_addr + 0xD8 + 0x10) == 0) {
      out.signature = string();
    } else {
      out.signature = string((char *)(service_addr + 0xD8),
                             *(DWORD *)(service_addr + 0xD8 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0xF0) == 0 ||
        *(DWORD *)(service_addr + 0xF0 + 0x10) == 0) {
      out.country = string();
    } else {
      out.country = string((char *)(service_addr + 0xF0),
                           *(DWORD *)(service_addr + 0xF0 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0x108) == 0 ||
        *(DWORD *)(service_addr + 0x108 + 0x10) == 0) {
      out.province = string();
    } else {
      out.province = string((char *)(service_addr + 0x108),
                            *(DWORD *)(service_addr + 0x108 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0x120) == 0 ||
        *(DWORD *)(service_addr + 0x120 + 0x10) == 0) {
      out.city = string();
    } else {
      out.city = string((char *)(service_addr + 0x120),
                        *(DWORD *)(service_addr + 0x120 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0x150) == 0 ||
        *(DWORD *)(service_addr + 0x150 + 0x10) == 0) {
      out.name = string();
    } else {
      out.name = string((char *)(service_addr + 0x150),
                        *(DWORD *)(service_addr + 0x150 + 0x10));
    }

    if (*(DWORD *)(service_addr + 0x304) == 0 ||
        *(DWORD *)(service_addr + 0x304 + 0x10) == 0) {
      out.head_img = string();
    } else {
      out.head_img = string(*(char **)(service_addr + 0x304),
                            *(DWORD *)(service_addr + 0x304 + 0x10));
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
    out.data_save_path =
        Wstring2String(wstring(data_save_path.ptr, data_save_path.length));
  }else {
    out.data_save_path = string();
  }

    if (current_data_path.ptr) {
    out.current_data_path =
        Wstring2String(wstring(current_data_path.ptr, current_data_path.length));
  }else {
    out.current_data_path = string();
  }
  return 1;
}


int CheckLogin(){
  DWORD base = GetWeChatWinBase();
  DWORD accout_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD service_addr = NULL;
  __asm {
      PUSHAD 
      CALL       accout_service_addr                          
      MOV        service_addr,EAX
      POPAD
  }
  if (service_addr) {
    return *(DWORD *)(service_addr + 0x4C8);
  }
  else {
    return 0;
  }
}


int Logout(){
  int success = 0;
  if(!CheckLogin()){
    return success;
  }
  DWORD base = GetWeChatWinBase();
  DWORD account_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD logout_addr = base + WX_LOGOUT_OFFSET;
  __asm{
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