#ifndef WECHAT_DATA_H_
#define WECHAT_DATA_H_

// #include <windows.h>

#include <vector>
using namespace std;
struct WeChatString {
  wchar_t *ptr;
  DWORD length;
  DWORD max_length;
  DWORD c_ptr = 0;
  DWORD c_len = 0;
  WeChatString() { WeChatString(NULL); }

  WeChatString(wstring &s) {
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
  vector<TableInfo> tables;
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
  string name;
  string city;
  string province;
  string country;
  string account;
  string wxid;
  string mobile;
  string  small_img;
  string big_img;
  string data_root_path;
  string data_save_path;
  string current_data_path;
};
#endif
