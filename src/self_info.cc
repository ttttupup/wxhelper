#include "pch.h"
#include "self_info.h"

#include "common.h"

#include "wechat_data.h"

#define WX_SELF_NAME_OFFSET 0x2bee198
#define WX_SELF_MOBILE_OFFSET 0x2BEE108
#define WX_SELF_CITY_OFFSET 0x2BEE168
#define WX_SELF_PROVINCE_OFFSET 0x2BEE150
#define WX_SELF_COUNTRY_OFFSET 0x2BEE138
#define WX_SELF_ACCOUNT_OFFSET 0x2BEE0F0
#define WX_SELF_ID_OFFSET 0x2BEE08C
#define WX_SELF_SMALL_IMG_OFFSET 0x2BEE34C
#define WX_SELF_BIG_IMG_OFFSET 0x2BEE364
#define WX_LOGIN_STATUS_OFFSET 0x2BEE4C0
#define WX_APP_DATA_ROOT_PATH_OFFSET 0x2c2f478
#define WX_APP_DATA_SAVE_PATH_OFFSET 0x2C10D04
#define WX_CURRENT_DATA_PATH_OFFSET 0x2C0EC38



int GetSelfInfo(SelfInfoInner &out) {
  DWORD base = GetWeChatWinBase();
#ifdef _DEBUG
  cout << "mobile:" << (char *)(base + WX_SELF_MOBILE_OFFSET) << endl;
  cout << "name:" << (char *)(base + WX_SELF_NAME_OFFSET) << endl;
  cout << "city:" << (char *)(base + WX_SELF_CITY_OFFSET) << endl;
  cout << "city:" << (char *)(base + WX_SELF_CITY_OFFSET) << endl;
  cout << "province:" << (char *)(base + WX_SELF_PROVINCE_OFFSET) << endl;
  cout << "country:" << (char *)(base + WX_SELF_COUNTRY_OFFSET) << endl;
  cout << "account:" << (char *)(base + WX_SELF_ACCOUNT_OFFSET) << endl;
  cout << "wxid:" << (char *)(base + WX_SELF_ID_OFFSET) << endl;
  cout << "small_img:" << (char *)(base + WX_SELF_SMALL_IMG_OFFSET) << endl;
  cout << "big_img:" << (char *)(base + WX_SELF_BIG_IMG_OFFSET) << endl;
#endif
  if (*(DWORD *)(base + WX_SELF_NAME_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_NAME_OFFSET + 0x10) == 0) {
    out.name = string();
  } else {
    out.name = string((char *)(base + WX_SELF_NAME_OFFSET),
                      *(DWORD *)(base + WX_SELF_NAME_OFFSET + 0x10));
  }
  if (*(DWORD *)(base + WX_SELF_MOBILE_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_MOBILE_OFFSET + 0x10) == 0) {
    out.mobile = string();
  } else {
    out.mobile = string((char *)(base + WX_SELF_MOBILE_OFFSET),
                       *(DWORD *)(base + WX_SELF_MOBILE_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_CITY_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_CITY_OFFSET + 0x10) == 0) {
    out.city = string();
  } else {
    out.city = string((char *)(base + WX_SELF_CITY_OFFSET),
                      *(DWORD *)(base + WX_SELF_CITY_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_PROVINCE_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_PROVINCE_OFFSET + 0x10) == 0) {
    out.province = string();
  } else {
    out.province = string((char *)(base + WX_SELF_PROVINCE_OFFSET),
                          *(DWORD *)(base + WX_SELF_PROVINCE_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_COUNTRY_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_COUNTRY_OFFSET + 0x10) == 0) {
    out.country = string();
  } else {
    out.country = string((char *)(base + WX_SELF_COUNTRY_OFFSET),
                         *(DWORD *)(base + WX_SELF_COUNTRY_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_ACCOUNT_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_ACCOUNT_OFFSET + 0x10) == 0) {
    out.account = string();
  } else {
    out.account = string(*(char **)(base + WX_SELF_ACCOUNT_OFFSET),
                         *(DWORD *)(base + WX_SELF_ACCOUNT_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_ID_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_ID_OFFSET + 0x10) == 0) {
    out.wxid = string();
  } else {
    out.wxid = string(*(char **)(base + WX_SELF_ID_OFFSET),
                      *(DWORD *)(base + WX_SELF_ID_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_SMALL_IMG_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_SMALL_IMG_OFFSET + 0x10) == 0) {
    out.small_img = string();
  } else {
    out.small_img = string(*(char **)(base + WX_SELF_SMALL_IMG_OFFSET),
                           *(DWORD *)(base + WX_SELF_SMALL_IMG_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_SELF_BIG_IMG_OFFSET) == 0 ||
      *(DWORD *)(base + WX_SELF_BIG_IMG_OFFSET + 0x10) == 0) {
    out.big_img = string();
  } else {
    out.big_img = string(*(char **)(base + WX_SELF_BIG_IMG_OFFSET),
                         *(DWORD *)(base + WX_SELF_BIG_IMG_OFFSET + 0x10));
  }

  if (*(DWORD *)(base + WX_APP_DATA_ROOT_PATH_OFFSET) == 0 ||
      *(DWORD *)(base + WX_APP_DATA_ROOT_PATH_OFFSET + 0x4) == 0) {
    out.data_root_path = string();
  } else {
    out.data_root_path = Wstring2String(wstring(*(wchar_t **)(base + WX_APP_DATA_ROOT_PATH_OFFSET),
                         *(DWORD *)(base + WX_APP_DATA_ROOT_PATH_OFFSET + 0x4)));
  }

  if (*(DWORD *)(base + WX_APP_DATA_SAVE_PATH_OFFSET) == 0 ||
      *(DWORD *)(base + WX_APP_DATA_SAVE_PATH_OFFSET + 0x4) == 0) {
    out.data_save_path = string();
  } else {
    out.data_save_path = Wstring2String(wstring(*(wchar_t **)(base + WX_APP_DATA_SAVE_PATH_OFFSET),
                         *(DWORD *)(base + WX_APP_DATA_SAVE_PATH_OFFSET + 0x4)));
  }
  if (*(DWORD *)(base + WX_CURRENT_DATA_PATH_OFFSET) == 0 ||
      *(DWORD *)(base + WX_CURRENT_DATA_PATH_OFFSET + 0x4) == 0) {
    out.current_data_path = string();
  } else {
    out.current_data_path = Wstring2String(wstring(*(wchar_t **)(base + WX_CURRENT_DATA_PATH_OFFSET),
                         *(DWORD *)(base + WX_CURRENT_DATA_PATH_OFFSET + 0x4)));
  }

  return 1;
}


int CheckLogin(){
  DWORD base = GetWeChatWinBase();
  return *(DWORD*) (base + WX_LOGIN_STATUS_OFFSET);
}