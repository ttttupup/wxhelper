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
  std::string head_img;
  std::string data_save_path;
  std::string signature;
  std::string current_data_path;
  std::string db_key;
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

struct AtInner{
  DWORD start;
  DWORD finsh;
  DWORD end;
};

struct ChatMsg {
  DWORD **field0_0x0;
  DWORD field1_0x4;
  ULONG64 sequence;
  DWORD field3_0x10;
  DWORD field4_0x14;
  ULONG64 msgSequence;
  DWORD localId;
  DWORD field7_0x24;
  DWORD field8_0x28;
  DWORD field9_0x2c;
  ULONG64 msgId; 
  DWORD type;
  DWORD isSendMsg;
  DWORD msgStatus;     
  DWORD timestamp;     
  WeChatString talker; 
  DWORD field16_0x5c;
  DWORD field17_0x60;
  DWORD field18_0x64;
  DWORD field19_0x68;
  DWORD field20_0x6c;
  WeChatString content; 
  DWORD field22_0x84;
  DWORD field23_0x88;
  DWORD field24_0x8c;
  DWORD field25_0x90;
  DWORD field26_0x94;
  DWORD field27_0x98;
  DWORD field28_0x9c;
  DWORD field29_0xa0;
  DWORD field30_0xa4;
  DWORD field31_0xa8;
  DWORD field32_0xac;
  DWORD field33_0xb0;
  DWORD field34_0xb4;
  DWORD field35_0xb8;
  DWORD field36_0xbc;
  DWORD field37_0xc0;
  DWORD field38_0xc4;
  DWORD field39_0xc8;
  DWORD field40_0xcc;
  DWORD field41_0xd0;
  DWORD field42_0xd4;
  DWORD field43_0xd8;
  DWORD field44_0xdc;
  DWORD field45_0xe0;
  DWORD field46_0xe4;
  DWORD field47_0xe8;
  DWORD field48_0xec;
  DWORD field49_0xf0;
  DWORD field50_0xf4;
  DWORD field51_0xf8;
  DWORD field52_0xfc;
  DWORD field53_0x100;
  DWORD field54_0x104;
  DWORD field55_0x108;
  DWORD field56_0x10c;
  DWORD field57_0x110;
  DWORD field58_0x114;
  DWORD field59_0x118;
  DWORD field60_0x11c;
  DWORD field61_0x120;
  DWORD field62_0x124;
  DWORD field63_0x128;
  DWORD field64_0x12c;
  DWORD field65_0x130;
  DWORD field66_0x134;
  DWORD field67_0x138;
  DWORD field68_0x13c;
  DWORD field69_0x140;
  DWORD field70_0x144;
  DWORD field71_0x148;
  DWORD field72_0x14c;
  DWORD field73_0x150;
  DWORD field74_0x154;
  DWORD field75_0x158;
  DWORD field76_0x15c;
  DWORD field77_0x160;
  DWORD field78_0x164;
  DWORD field79_0x168;
  DWORD field80_0x16c;
  DWORD field81_0x170;
  WeChatString fromGroup;
  WeChatString sign;
  WeChatString thumbPath;
  WeChatString path;
  DWORD field86_0x1c4;
  DWORD field87_0x1c8;
  DWORD field88_0x1cc;
  DWORD field89_0x1d0;
  DWORD field90_0x1d4;
  DWORD field91_0x1d8;
  DWORD field92_0x1dc;
  DWORD field93_0x1e0;
  DWORD field94_0x1e4;
  DWORD field95_0x1e8;
  DWORD field96_0x1ec;
  WeChatString signature;
  DWORD field98_0x204;
  DWORD field99_0x208;
  DWORD field100_0x20c;
  DWORD field101_0x210;
  DWORD field102_0x214;
  DWORD field103_0x218;
  DWORD field104_0x21c;
  DWORD field105_0x220;
  DWORD field106_0x224;
  DWORD field107_0x228;
  DWORD field108_0x22c;
  DWORD field109_0x230;
  DWORD field110_0x234;
  DWORD field111_0x238;
  DWORD field112_0x23c;
  DWORD field113_0x240;
  DWORD field114_0x244;
  DWORD field115_0x248;
  DWORD field116_0x24c;
  DWORD field117_0x250;
  DWORD field118_0x254;
  DWORD field119_0x258;
  DWORD field120_0x25c;
  DWORD field121_0x260;
  DWORD field122_0x264;
  DWORD field123_0x268;
  DWORD field124_0x26c;
  DWORD field125_0x270;
  DWORD field126_0x274;
  DWORD field127_0x278;
  DWORD field128_0x27c;
  DWORD field129_0x280;
  DWORD field130_0x284;
  DWORD field131_0x288;
  DWORD field132_0x28c;
  DWORD field133_0x290;
  DWORD field134_0x294;
  DWORD field135_0x298;
  DWORD field136_0x29c;
  DWORD field137_0x2a0;
  DWORD field138_0x2a4;
  DWORD field139_0x2a8;
  DWORD field140_0x2ac;
  DWORD field141_0x2b0;
  int field142_0x2b4;
};

#endif
