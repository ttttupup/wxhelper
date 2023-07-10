#ifndef WXHELPER_WECHAT_FUNCTION_H_
#define WXHELPER_WECHAT_FUNCTION_H_

namespace wxhelper {
namespace common {
  /***************************sqlite3***************************************/
#define SQLITE_OK 0 /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR 1       /* Generic error */
#define SQLITE_INTERNAL 2    /* Internal logic error in SQLite */
#define SQLITE_PERM 3        /* Access permission denied */
#define SQLITE_ABORT 4       /* Callback routine requested an abort */
#define SQLITE_BUSY 5        /* The database file is locked */
#define SQLITE_LOCKED 6      /* A table in the database is locked */
#define SQLITE_NOMEM 7       /* A malloc() failed */
#define SQLITE_READONLY 8    /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT 9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR 10      /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT 11    /* The database disk image is malformed */
#define SQLITE_NOTFOUND 12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL 13       /* Insertion failed because database is full */
#define SQLITE_CANTOPEN 14   /* Unable to open the database file */
#define SQLITE_PROTOCOL 15   /* Database lock protocol error */
#define SQLITE_EMPTY 16      /* Internal use only */
#define SQLITE_SCHEMA 17     /* The database schema changed */
#define SQLITE_TOOBIG 18     /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT 19 /* Abort due to constraint violation */
#define SQLITE_MISMATCH 20   /* Data type mismatch */
#define SQLITE_MISUSE 21     /* Library used incorrectly */
#define SQLITE_NOLFS 22      /* Uses OS features not supported on host */
#define SQLITE_AUTH 23       /* Authorization denied */
#define SQLITE_FORMAT 24     /* Not used */
#define SQLITE_RANGE 25      /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB 26     /* File opened that is not a database file */
#define SQLITE_NOTICE 27     /* Notifications from sqlite3_log() */
#define SQLITE_WARNING 28    /* Warnings from sqlite3_log() */
#define SQLITE_ROW 100       /* sqlite3_step() has another row ready */
#define SQLITE_DONE 101      /* sqlite3_step() has finished executing */
/* end-of-error-codes */

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#define SQLITE_TEXT     3




typedef int (*sqlite3_callback)(void*, int, char**, char**);

typedef int(__cdecl* sqlite3_exec)(UINT64,            /* An open database */
                                   const char* sql,  /* SQL to be evaluated */
                                   sqlite3_callback, /* Callback function */
                                   void*,        /* 1st argument to callback */
                                   char** errmsg /* Error msg written here */
);

typedef int(__cdecl* sqlite3_prepare)(
    UINT64 db,          /* Database handle */
    const char* zSql,   /* SQL statement, UTF-8 encoded */
    int nByte,          /* Maximum length of zSql in bytes. */
    UINT64** ppStmt,     /* OUT: Statement handle */
    const char** pzTail /* OUT: Pointer to unused portion of zSql */
);
typedef int(__cdecl* sqlite3_open)(const char* filename, UINT64** ppDb);

typedef int(__cdecl* sqlite3_sleep)(int);
typedef int(__cdecl* sqlite3_errcode)(UINT64* db);
typedef int(__cdecl* sqlite3_close)(UINT64*);

typedef int(__cdecl* sqlite3_step)(UINT64*);
typedef int(__cdecl* sqlite3_column_count)(UINT64* pStmt);
typedef const char*(__cdecl* sqlite3_column_name)(UINT64*, int N);
typedef int(__cdecl* sqlite3_column_type)(UINT64*, int iCol);
typedef const void*(__cdecl* sqlite3_column_blob)(UINT64*, int iCol);
typedef int(__cdecl* sqlite3_column_bytes)(UINT64*, int iCol);
typedef int(__cdecl* sqlite3_finalize)(UINT64* pStmt);


/***************************sqlite3  end*************************************/

struct TableInfo {
  char *name;
  INT64 name_len;
  char *table_name;
  INT64 table_name_len;
  char *sql;
  INT64 sql_len;
  char *rootpage;
  INT64 rootpage_len;
};

struct DatabaseInfo {
  UINT64 handle = 0;
  wchar_t *db_name = NULL;
  INT64 db_name_len = 0;
  std::vector<TableInfo> tables;
  INT64 count = 0;
  INT64 extrainfo = 0;
};


struct SqlResult {
  char *column_name;
  INT64 column_name_len;
  char *content;
  INT64 content_len;
  BOOL is_blob;
};


struct InnerMessageStruct {
  char *buffer;
  int length;
  ~InnerMessageStruct() {
    if (this->buffer != NULL) {
      delete[] this->buffer;
      this->buffer = NULL;
    }
  }
};

struct SelfInfoInner {
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

struct ContactInner {
  std::string wxid;
  std::string custom_account;
  std::string encrypt_name;
  std::string nickname;
  std::string pinyin;
  std::string pinyin_all;
  DWORD type;
  DWORD verify_flag;
  DWORD reserved1;
  DWORD reserved2;
  ContactInner(){
    wxid = "";
    custom_account = "";
    encrypt_name = "";
    nickname ="";
    pinyin ="";
    pinyin_all ="";
    type = -1;
    verify_flag = -1;
    reserved1 = -1;
    reserved2 = -1;
  }
};

struct ChatRoomInfoInner {
  std::string chat_room_id;
  std::string notice;
  std::string admin;
  std::string xml;
  ChatRoomInfoInner(){
    chat_room_id = "";
    notice = "";
    admin = "";
    xml = "";
  }

};

struct VectorInner {
#ifdef _DEBUG
  INT64 head;
#endif
  INT64 start;
  INT64 finsh;
  INT64 end;
};

}  // namespace common
namespace V3_9_5_81 {
namespace function {


typedef UINT64(*__GetAccountService)();
typedef UINT64(*__GetDataSavePath)(UINT64);
typedef UINT64(*__GetCurrentDataPath)(UINT64);
typedef UINT64(*__GetSendMessageMgr)();
typedef UINT64 (*__SendTextMsg)(UINT64, UINT64, UINT64, UINT64, UINT64, UINT64,
                                UINT64, UINT64);
typedef UINT64 (*__FreeChatMsg)(UINT64);

typedef UINT64 (*__SendImageMsg)(UINT64, UINT64, UINT64, UINT64, UINT64);
typedef UINT64 (*__NewChatMsg)(UINT64);
typedef UINT64 (*__SendFile)(UINT64, UINT64, UINT64, UINT64, UINT64,UINT64, UINT64, UINT64, UINT64, UINT64, UINT64, UINT64);
typedef UINT64(*__GetAppMsgMgr)();
typedef UINT64(*__OperatorNew)(UINT64);

typedef UINT64(*__Free)();
typedef UINT64 (*__GetContactMgr)();
typedef UINT64 (*__GetContactList)(UINT64,UINT64);

typedef UINT64 (*__GetChatRoomMgr)();
typedef UINT64 (*__NewChatRoomInfo)(UINT64);
typedef UINT64 (*__FreeChatRoomInfo)(UINT64);
typedef UINT64 (*__GetChatRoomDetailInfo)(UINT64,UINT64,UINT64,UINT64);
typedef UINT64 (*__DoAddMemberToChatRoom)(UINT64,UINT64,UINT64,UINT64);

typedef UINT64 (*__DoModChatRoomMemberNickName)(UINT64,UINT64,UINT64,UINT64);


}  // namespace function
namespace prototype {

#include <string>
#include <vector>

struct WeChatString {
  wchar_t *ptr;
  DWORD length;
  DWORD max_length;
  INT64 c_ptr = 0;
  DWORD c_len = 0;
  WeChatString() { WeChatString(NULL); }

  WeChatString(const std::wstring &s) {
    ptr = (wchar_t *)(s.c_str());
    length = static_cast<DWORD>(s.length());
    max_length = static_cast<DWORD>(s.length());
  }
  WeChatString(const wchar_t *pStr) { WeChatString((wchar_t *)pStr); }
  WeChatString(int tmp) {
    ptr = NULL;
    length = 0x0;
    max_length = 0x0;
  }
  WeChatString(wchar_t *pStr) {
    ptr = pStr;
    length = static_cast<DWORD>(wcslen(pStr));
    max_length = static_cast<DWORD>(wcslen(pStr));
  }
  void set_value(const wchar_t *pStr) {
    ptr = (wchar_t *)pStr;
    length = static_cast<DWORD>(wcslen(pStr));
    max_length = static_cast<DWORD>(wcslen(pStr) * 2);
  }
};

}  // namespace prototype
namespace offset {
const UINT64 kGetAccountServiceMgr = 0x8c1230;
const UINT64 kSyncMsg = 0xc39680;
const UINT64 kSyncMsgNext = 0xc39680;
const UINT64 kGetCurrentDataPath = 0xf5d130;
const UINT64 kGetAppDataSavePath = 0x12d7040;
const UINT64 kGetSendMessageMgr = 0x8c00e0;
const UINT64 kSendTextMsg = 0xfcd8d0;
const UINT64 kFreeChatMsg = 0x8aaa00;

const UINT64 kDoAddMsg = 0x1010d80;
const UINT64 kSendImageMsg = 0xfc3d30;
const UINT64 kChatMsgInstanceCounter = 0x8c7fd0;
const UINT64 kSendFileMsg = 0xdd27f0;
const UINT64 kGetAppMsgMgr = 0x8c33f0;
const UINT64 kGetContactMgr = 0x8ae3d0;
const UINT64 kGetContactList = 0xeab270;

const UINT64 k_sqlite3_exec = 0x252e340;
const UINT64 k_sqlite3_prepare =  0x2535eb0;
const UINT64 k_sqlite3_open = 0x256d6b0;
const UINT64 k_sqlite3_backup_init= 0x24e8450;
const UINT64 k_sqlite3_errcode = 0x256bfb0;
const UINT64 k_sqlite3_close = 0x256a110;
const UINT64 k_sqlite3_step =  0x24f2350;
const UINT64 k_sqlite3_column_count =  0x24f2b70;
const UINT64 k_sqlite3_column_name =  0x24f3570;
const UINT64 k_sqlite3_column_type =  0x24f33c0;
const UINT64 k_sqlite3_column_blob = 0x24f2ba0;
const UINT64 k_sqlite3_column_bytes =  0x24f2c90;
const UINT64 k_sqlite3_finalize =  0x24f1400;

const UINT64 kGPInstance = 0x3a6f908;
const UINT64 kMicroMsgDB = 0xb8;
const UINT64 kChatMsgDB = 0x2c8;
const UINT64 kMiscDB = 0x5f0;
const UINT64 kEmotionDB = 0x838;
const UINT64 kMediaDB = 0xef8;
const UINT64 kBizchatMsgDB = 0x1a70;
const UINT64 kFunctionMsgDB = 0x1b48;
const UINT64 kDBName = 0x28;
const UINT64 kStorageStart = 0x0;
const UINT64 kStorageEnd = 0x0;
const UINT64 kMultiDBMgr = 0x3acfb68;
const UINT64 kPublicMsgMgr = 0x3acc268;
const UINT64 kFavoriteStorageMgr = 0x3acf0d0;

const UINT64 kChatRoomMgr = 0x8e9d30;
const UINT64 kGetChatRoomDetailInfo = 0xe73590;
const UINT64 kNewChatRoomInfo = 0x12006b0;
const UINT64 kFreeChatRoomInfo = 0x1200890;
const UINT64 kDoAddMemberToChatRoom = 0xe63c70;
const UINT64 kDoModChatRoomMemberNickName = 0xe6db00;
}  // namespace offset
}  // namespace V3_9_5_81

}  // namespace wxhelper

#endif