#ifndef WECHAT_DATA_H_
#define WECHAT_DATA_H_

// #include <windows.h>

#include <vector>
struct WeChatString {
  wchar_t *ptr;
  DWORD length;
  DWORD max_length;
  DWORD c_ptr = 0;
  DWORD c_len = 0;
  WeChatString() { WeChatString(NULL); }

  WeChatString(std::wstring &s) {
    ptr = (wchar_t *)(s.c_str());
    length = s.length();
    max_length = s.length() * 2;
  }
  WeChatString(const wchar_t *pStr) { WeChatString((wchar_t *)pStr); }
  WeChatString(int tmp) {
    ptr = NULL;
    length = 0x0;
    max_length = 0x0;
  }
  WeChatString(wchar_t *pStr) {
    ptr = pStr;
    length = wcslen(pStr);
    max_length = wcslen(pStr) * 2;
  }
  void set_value(const wchar_t *pStr) {
    ptr = (wchar_t *)pStr;
    length = wcslen(pStr);
    max_length = wcslen(pStr) * 2;
  }
};

struct TableInfo {
  char *name;
  DWORD name_len;
  char *table_name;
  DWORD table_name_len;
  char *sql;
  DWORD sql_len;
  char *rootpage;
  DWORD rootpage_len;
};

struct DatabaseInfo {
  DWORD handle = 0;
  wchar_t *db_name = NULL;
  DWORD db_name_len = 0;
  std::vector<TableInfo> tables;
  DWORD count = 0;
  DWORD extrainfo = 0;
};

struct Contact {
  WeChatString wxid;
  WeChatString custom_account;
  WeChatString encrypt_name;
  WeChatString nick_name;
  WeChatString pinyin;
  WeChatString pinyin_all;
  int del_flag;
  int type;
  int verify_flag;
};

struct ChatRoomInfo {
  DWORD vftable;
  WeChatString chat_room_id;
  WeChatString notice;
  WeChatString admin;
  DWORD filed_40;
  DWORD filed_44;
  DWORD filed_48;
  DWORD filed_4C;
  WeChatString xml;
  DWORD filed_64;
  DWORD filed_68;
  DWORD filed_6C;
  DWORD filed_70;
  DWORD filed_74;
  DWORD filed_78;
  DWORD filed_7C;
  DWORD filed_80;
  DWORD filed_84;
  DWORD filed_88;
  DWORD filed_8c;
  DWORD filed_90;
  DWORD filed_94;
  DWORD filed_98;
  DWORD filed_9C;
  DWORD filed_A0;
};

struct ChatRoomInfoInner {
  WeChatString chat_room_id;
  WeChatString notice;
  WeChatString admin;
  WeChatString xml;

  ~ChatRoomInfoInner(){
      if(chat_room_id.ptr){
        delete []chat_room_id.ptr;
        chat_room_id.ptr = nullptr;
      }
      if(notice.ptr){
        delete []notice.ptr;
        notice.ptr = nullptr;
      }
      if(admin.ptr){
        delete []admin.ptr;
        admin.ptr = nullptr;
      }
      if(xml.ptr){
        delete []xml.ptr;
        xml.ptr = nullptr;
      }
  }
};

struct VectorInner {
#ifdef _DEBUG
  DWORD head;
#endif
  DWORD start;
  DWORD finsh;
  DWORD end;
};

struct ChatRoomInner{
  char* members;
  wchar_t* chat_room;
  wchar_t* admin;
  ~ChatRoomInner(){
    delete []members;
    delete []chat_room;
    delete []admin;
  }
};

struct SelfInfoInner{
  std::string name;
  std::string city;
  std::string province;
  std::string country;
  std::string account;
  std::string wxid;
  std::string mobile;
  std::string  small_img;
  std::string big_img;
  std::string data_root_path;
  std::string data_save_path;
  std::string current_data_path;
};

struct UserInfo {
  int error_code;
  wchar_t *keyword;
  int keyword_len;
  wchar_t *v3;
  int v3_len;
  wchar_t *nickname;
  int nickname_len;
  wchar_t *signature;
  int signature_len;
  wchar_t *v2;
  int v2_len;
  wchar_t *nation;
  int nation_len;
  wchar_t *province;
  int province_len;
  wchar_t *city;
  int city_len;
  wchar_t *big_image;
  int big_image_len;
  wchar_t *small_image;
  int small_image_len;
  DWORD sex;
  BOOL over;
};
#endif
