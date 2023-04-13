#ifndef WXHELPER_WECHAT_FUNCTION_H_
#define WXHELPER_WECHAT_FUNCTION_H_
#include <vector>
#include <string>

// snsDataMgr
#define WX_SNS_DATA_MGR_OFFSET 0xc39680
// chatRoomMgr
#define WX_CHAT_ROOM_MGR_OFFSET 0x78cf20
// contactMgr
#define WX_CONTACT_MGR_OFFSET 0x75a4a0
// syncMgr
#define WX_SYNC_MGR_OFFSET 0xa87fd0
// preDownloadMgr
#define WX_GET_PRE_DOWNLOAD_MGR_OFFSET 0x80f110
// chatMgr
#define WX_CHAT_MGR_OFFSET 0x792700
// videoMgr
#define WX_VIDEO_MGR_OFFSET 0x829820
// patMgr
#define  WX_PAT_MGR_OFFSET  0x931730
// searchContactMgr
#define WX_SEARCH_CONTACT_MGR_OFFSET 0xa6cb00
// appMsgMgr
#define WX_APP_MSG_MGR_OFFSET   0x76ae20
// sendMessageMgr
#define WX_SEND_MESSAGE_MGR_OFFSET 0x768140


// setChatMsgValue
#define WX_INIT_CHAT_MSG_OFFSET 0xf59e40

// chatMsg
#define WX_NEW_CHAT_MSG_OFFSET 0x76f010
#define WX_FREE_CHAT_MSG_OFFSET 0x756960
#define WX_FREE_CHAT_MSG_2_OFFSET 0x6f4ea0
#define WX_FREE_CHAT_MSG_INSTANCE_COUNTER_OFFSET 0x756e30


//sns
#define WX_SNS_GET_FIRST_PAGE_OFFSET 0x14e2140
#define WX_SNS_GET_NEXT_PAGE_OFFSET  0x14e21e0

//chat room
#define WX_GET_CHAT_ROOM_DETAIL_INFO_OFFSET 0xbde090
// chatRoomInfo
#define WX_NEW_CHAT_ROOM_INFO_OFFSET 0xe99c40
#define WX_FREE_CHAT_ROOM_INFO_OFFSET 0xe99f40
#define WX_DEL_CHAT_ROOM_MEMBER_OFFSET 0xbd22a0
#define WX_ADD_MEMBER_TO_CHAT_ROOM_OFFSET 0xbd1dc0


// chatRoom
#define WX_INIT_CHAT_ROOM_OFFSET 0xe97890
#define WX_FREE_CHAT_ROOM_OFFSET 0xe97ab0

#define WX_GET_MEMBER_FROM_CHAT_ROOM_OFFSET 0xbdf260
#define WX_MOD_CHAT_ROOM_MEMBER_NICK_NAME_OFFSET 0xbd9680

#define WX_TOP_MSG_OFFSET 0xbe1840
#define WX_REMOVE_TOP_MSG_OFFSET 0xbe1620

#define WX_GET_MEMBER_NICKNAME_OFFSET  0xbdf3f0

#define WX_FREE_CONTACT_OFFSET  0xea7880

// wcpayinfo
#define WX_NEW_WCPAYINFO_OFFSET 0x7b2e60
#define WX_FREE_WCPAYINFO_OFFSET 0x79c250
#define WX_CONFIRM_RECEIPT_OFFSET 0x15e2c20


//contact
#define WX_CONTACT_GET_LIST_OFFSET 0xc089f0
#define WX_CONTACT_DEL_OFFSET 0xb9b3b0

#define WX_SET_VALUE_OFFSET 0x1f80900
#define WX_DO_DEL_CONTACT_OFFSET 0xca6480
#define WX_GET_CONTACT_OFFSET  0xc04e00
#define WX_DO_VERIFY_USER_OFFSET  0xc02100
#define WX_VERIFY_MSG_OFFSET  0xf59d40
#define WX_VERIFY_OK_OFFSET  0xa18bd0
#define WX_NEW_ADD_FRIEND_HELPER_OFFSET 0xa17d50
#define WX_FREE_ADD_FRIEND_HELPER_OFFSET 0xa17e70

// pushAttachTask


#define WX_PUSH_ATTACH_TASK_OFFSET 0x82bb40

#define WX_FREE_CHAT_MSG_OFFSET 0x756960
#define WX_GET_MGR_BY_PREFIX_LOCAL_ID_OFFSET 0xbc0370
#define WX_GET_CURRENT_DATA_PATH_OFFSET 0xc872c0
#define WX_APP_MSG_INFO_OFFSET 0x7b3d20
#define WX_GET_APP_MSG_XML_OFFSET 0xe628a0
#define WX_FREE_APP_MSG_INFO_OFFSET 0x79d900
#define WX_PUSH_THUMB_TASK_OFFSET 0x82ba40
#define WX_DOWNLOAD_VIDEO_IMG_OFFSET 0xd46c30





// pat
#define  WX_SEND_PAT_MSG_OFFSET  0x1421940
#define  WX_RET_OFFSET   0x1D58751


//search hook
#define WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSET 0xe17054
#define WX_SEARCH_CONTACT_ERROR_CODE_HOOK_NEXT_OFFSET 0xf57a20
#define WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSET 0xa8ceb0
#define WX_SEARCH_CONTACT_DETAIL_HOOK_NEXT_OFFSET 0xa8d100
#define WX_SEARCH_CONTACT_OFFSET 0xcd1510



//login
#define WX_LOGOUT_OFFSET 0xe58870
#define WX_ACCOUNT_SERVICE_OFFSET 0x768c80
#define WX_GET_APP_DATA_SAVE_PATH_OFFSET 0xf3a610
#define WX_GET_CURRENT_DATA_PATH_OFFSET 0xc872c0


//forward
#define WX_FORWARD_MSG_OFFSET 0xce6730
// send file
#define WX_SEND_FILE_OFFSET     0xb6d1f0
// send image
#define WX_SEND_IMAGE_OFFSET  0xce6640
// send text
#define WX_SEND_TEXT_OFFSET 0xce6c80


//ocr
#define WX_INIT_OBJ_OFFSET 0x7a98f0
#define WX_OCR_MANAGER_OFFSET 0x7ae470
#define WX_DO_OCR_TASK_OFFSET 0x13230c0


//storage

#define CONTACT_G_PINSTANCE_OFFSET 0x2ffddc8
#define DB_MICRO_MSG_OFFSET 0x68
#define DB_CHAT_MSG_OFFSET 0x1C0
#define DB_MISC_OFFSET 0x3D8
#define DB_EMOTION_OFFSET 0x558
#define DB_MEDIA_OFFSET 0x9B8
#define DB_BIZCHAT_MSG_OFFSET 0x1120
#define DB_FUNCTION_MSG_OFFSET 0x11B0
#define DB_NAME_OFFSET 0x14

#define STORAGE_START_OFFSET  0x13f8
#define STORAGE_END_OFFSET  0x13fc

#define PUBLIC_MSG_MGR_OFFSET  0x303df74
#define MULTI_DB_MSG_MGR_OFFSET  0x30403b8
#define FAVORITE_STORAGE_MGR_OFFSET  0x303fd40
#define FTS_FAVORITE_MGR_OFFSET  0x2ffe908

#define OP_LOG_STORAGE_VFTABLE 0x2AD3A20
#define CHAT_MSG_STORAGE_VFTABLE 0x2AC10F0
#define CHAT_CR_MSG_STORAGE_VFTABLE 0x2ABEF14
#define SESSION_STORAGE_VFTABLE 0x2AD3578
#define APP_INFO_STORAGE_VFTABLE 0x2ABCC58
#define HEAD_IMG_STORAGE_VFTABLE 0x2ACD9DC
#define HEAD_IMG_URL_STORAGE_VFTABLE 0x2ACDF70

#define BIZ_INFO_STORAGE_VFTABLE 0x2ABD718
#define TICKET_INFO_STORAGE_VFTABLE 0x2AD5400
#define CHAT_ROOM_STORAGE_VFTABLE 0x2AC299C
#define CHAT_ROOM_INFO_STORAGE_VFTABLE 0x2AC245C
#define MEDIA_STORAGE_VFTABLE 0x2ACE998
#define NAME_2_ID_STORAGE_VFTABLE 0x2AD222C
#define EMOTION_PACKAGE_STORAGE_VFTABLE 0x2AC6400

#define EMOTION_STORAGE_VFTABLE 0x2AC7018
#define BUFINFO_STORAGE_VFTABLE 0x2AC3178

#define CUSTOM_EMOTION_STORAGE_VFTABLE 0x2AC4E90
#define DEL_SESSIONINFO_STORAGE_VFTABLE 0x2AC5F98
#define FUNCTION_MSG_STORAGE_VFTABLE 0x2ACD10C

#define FUNCTION_MSG_TASK_STORAGE_VFTABLE 0x2ACC5C8
#define REVOKE_MSG_STORAGE_VFTABLE 0x2AD27BC



/*******************hook*********************************************/  


// hook image
#define WX_HOOK_IMG_OFFSET 0xd723dc
#define WX_HOOK_IMG_NEXT_OFFSET 0xe91d90



// hook log
#define WX_HOOK_LOG_OFFSET 0xf57d67
#define WX_HOOK_LOG_NEXT_OFFSET 0x240ea71

// hook msg

#define WX_RECV_MSG_HOOK_OFFSET 0xd19a0b
#define WX_RECV_MSG_HOOK_NEXT_OFFSET 0x756960
#define WX_SNS_HOOK_OFFSET  0x14f9e15
#define WX_SNS_HOOK_NEXT_OFFSET 0x14fa0a0


// hook voice
#define WX_HOOK_VOICE_OFFSET 0xd4d8d8
#define WX_HOOK_VOICE_NEXT_OFFSET 0x203d130
#define WX_SELF_ID_OFFSET 0x2FFD484 

/*******************hook end*********************************************/  


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

/*
** CAPI3REF: Extended Result Codes
** KEYWORDS: {extended result code definitions}
**
** In its default configuration, SQLite API routines return one of 30 integer
** [result codes].  However, experience has shown that many of
** these result codes are too coarse-grained.  They do not provide as
** much information about problems as programmers might like.  In an effort to
** address this, newer versions of SQLite (version 3.3.8 [dateof:3.3.8]
** and later) include
** support for additional result codes that provide more detailed information
** about errors. These [extended result codes] are enabled or disabled
** on a per database connection basis using the
** [sqlite3_extended_result_codes()] API.  Or, the extended code for
** the most recent error can be obtained using
** [sqlite3_extended_errcode()].
*/
#define SQLITE_ERROR_MISSING_COLLSEQ (SQLITE_ERROR | (1 << 8))
#define SQLITE_ERROR_RETRY (SQLITE_ERROR | (2 << 8))
#define SQLITE_ERROR_SNAPSHOT (SQLITE_ERROR | (3 << 8))
#define SQLITE_IOERR_READ (SQLITE_IOERR | (1 << 8))
#define SQLITE_IOERR_SHORT_READ (SQLITE_IOERR | (2 << 8))
#define SQLITE_IOERR_WRITE (SQLITE_IOERR | (3 << 8))
#define SQLITE_IOERR_FSYNC (SQLITE_IOERR | (4 << 8))
#define SQLITE_IOERR_DIR_FSYNC (SQLITE_IOERR | (5 << 8))
#define SQLITE_IOERR_TRUNCATE (SQLITE_IOERR | (6 << 8))
#define SQLITE_IOERR_FSTAT (SQLITE_IOERR | (7 << 8))
#define SQLITE_IOERR_UNLOCK (SQLITE_IOERR | (8 << 8))
#define SQLITE_IOERR_RDLOCK (SQLITE_IOERR | (9 << 8))
#define SQLITE_IOERR_DELETE (SQLITE_IOERR | (10 << 8))
#define SQLITE_IOERR_BLOCKED (SQLITE_IOERR | (11 << 8))
#define SQLITE_IOERR_NOMEM (SQLITE_IOERR | (12 << 8))
#define SQLITE_IOERR_ACCESS (SQLITE_IOERR | (13 << 8))
#define SQLITE_IOERR_CHECKRESERVEDLOCK (SQLITE_IOERR | (14 << 8))
#define SQLITE_IOERR_LOCK (SQLITE_IOERR | (15 << 8))
#define SQLITE_IOERR_CLOSE (SQLITE_IOERR | (16 << 8))
#define SQLITE_IOERR_DIR_CLOSE (SQLITE_IOERR | (17 << 8))
#define SQLITE_IOERR_SHMOPEN (SQLITE_IOERR | (18 << 8))
#define SQLITE_IOERR_SHMSIZE (SQLITE_IOERR | (19 << 8))
#define SQLITE_IOERR_SHMLOCK (SQLITE_IOERR | (20 << 8))
#define SQLITE_IOERR_SHMMAP (SQLITE_IOERR | (21 << 8))
#define SQLITE_IOERR_SEEK (SQLITE_IOERR | (22 << 8))
#define SQLITE_IOERR_DELETE_NOENT (SQLITE_IOERR | (23 << 8))
#define SQLITE_IOERR_MMAP (SQLITE_IOERR | (24 << 8))
#define SQLITE_IOERR_GETTEMPPATH (SQLITE_IOERR | (25 << 8))
#define SQLITE_IOERR_CONVPATH (SQLITE_IOERR | (26 << 8))
#define SQLITE_IOERR_VNODE (SQLITE_IOERR | (27 << 8))
#define SQLITE_IOERR_AUTH (SQLITE_IOERR | (28 << 8))
#define SQLITE_IOERR_BEGIN_ATOMIC (SQLITE_IOERR | (29 << 8))
#define SQLITE_IOERR_COMMIT_ATOMIC (SQLITE_IOERR | (30 << 8))
#define SQLITE_IOERR_ROLLBACK_ATOMIC (SQLITE_IOERR | (31 << 8))
#define SQLITE_IOERR_DATA (SQLITE_IOERR | (32 << 8))
#define SQLITE_IOERR_CORRUPTFS (SQLITE_IOERR | (33 << 8))
#define SQLITE_LOCKED_SHAREDCACHE (SQLITE_LOCKED | (1 << 8))
#define SQLITE_LOCKED_VTAB (SQLITE_LOCKED | (2 << 8))
#define SQLITE_BUSY_RECOVERY (SQLITE_BUSY | (1 << 8))
#define SQLITE_BUSY_SNAPSHOT (SQLITE_BUSY | (2 << 8))
#define SQLITE_BUSY_TIMEOUT (SQLITE_BUSY | (3 << 8))
#define SQLITE_CANTOPEN_NOTEMPDIR (SQLITE_CANTOPEN | (1 << 8))
#define SQLITE_CANTOPEN_ISDIR (SQLITE_CANTOPEN | (2 << 8))
#define SQLITE_CANTOPEN_FULLPATH (SQLITE_CANTOPEN | (3 << 8))
#define SQLITE_CANTOPEN_CONVPATH (SQLITE_CANTOPEN | (4 << 8))
#define SQLITE_CANTOPEN_DIRTYWAL (SQLITE_CANTOPEN | (5 << 8)) /* Not Used */
#define SQLITE_CANTOPEN_SYMLINK (SQLITE_CANTOPEN | (6 << 8))
#define SQLITE_CORRUPT_VTAB (SQLITE_CORRUPT | (1 << 8))
#define SQLITE_CORRUPT_SEQUENCE (SQLITE_CORRUPT | (2 << 8))
#define SQLITE_CORRUPT_INDEX (SQLITE_CORRUPT | (3 << 8))
#define SQLITE_READONLY_RECOVERY (SQLITE_READONLY | (1 << 8))
#define SQLITE_READONLY_CANTLOCK (SQLITE_READONLY | (2 << 8))
#define SQLITE_READONLY_ROLLBACK (SQLITE_READONLY | (3 << 8))
#define SQLITE_READONLY_DBMOVED (SQLITE_READONLY | (4 << 8))
#define SQLITE_READONLY_CANTINIT (SQLITE_READONLY | (5 << 8))
#define SQLITE_READONLY_DIRECTORY (SQLITE_READONLY | (6 << 8))
#define SQLITE_ABORT_ROLLBACK (SQLITE_ABORT | (2 << 8))
#define SQLITE_CONSTRAINT_CHECK (SQLITE_CONSTRAINT | (1 << 8))
#define SQLITE_CONSTRAINT_COMMITHOOK (SQLITE_CONSTRAINT | (2 << 8))
#define SQLITE_CONSTRAINT_FOREIGNKEY (SQLITE_CONSTRAINT | (3 << 8))
#define SQLITE_CONSTRAINT_FUNCTION (SQLITE_CONSTRAINT | (4 << 8))
#define SQLITE_CONSTRAINT_NOTNULL (SQLITE_CONSTRAINT | (5 << 8))
#define SQLITE_CONSTRAINT_PRIMARYKEY (SQLITE_CONSTRAINT | (6 << 8))
#define SQLITE_CONSTRAINT_TRIGGER (SQLITE_CONSTRAINT | (7 << 8))
#define SQLITE_CONSTRAINT_UNIQUE (SQLITE_CONSTRAINT | (8 << 8))
#define SQLITE_CONSTRAINT_VTAB (SQLITE_CONSTRAINT | (9 << 8))
#define SQLITE_CONSTRAINT_ROWID (SQLITE_CONSTRAINT | (10 << 8))
#define SQLITE_CONSTRAINT_PINNED (SQLITE_CONSTRAINT | (11 << 8))
#define SQLITE_CONSTRAINT_DATATYPE (SQLITE_CONSTRAINT | (12 << 8))
#define SQLITE_NOTICE_RECOVER_WAL (SQLITE_NOTICE | (1 << 8))
#define SQLITE_NOTICE_RECOVER_ROLLBACK (SQLITE_NOTICE | (2 << 8))
#define SQLITE_WARNING_AUTOINDEX (SQLITE_WARNING | (1 << 8))
#define SQLITE_AUTH_USER (SQLITE_AUTH | (1 << 8))
#define SQLITE_OK_LOAD_PERMANENTLY (SQLITE_OK | (1 << 8))
#define SQLITE_OK_SYMLINK (SQLITE_OK | (2 << 8)) /* internal use only */


#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#define SQLITE_TEXT     3

#define SQLITE3_EXEC_OFFSET 0x1e24f70
#define SQLITE3_BACKUP_INIT_OFFSET 0x1dea900
#define SQLITE3_PREPARE_OFFSET 0x1e2b8c0
#define SQLITE3_OPEN_OFFSET 0x1e598b0
#define SQLITE3_BACKUP_STEP_OFFSET 0x1dead00
#define SQLITE3_BACKUP_REMAINING_OFFSET 0x1deb440
#define SQLITE3_BACKUP_PAGECOUNT_OFFSET 0x1deb450
#define SQLITE3_BACKUP_FINISH_OFFSET 0x1deb340
#define SQLITE3_SLEEP_OFFSET 0x1e5a0f0
#define SQLITE3_ERRCODE_OFFSET 0x1e58550
#define SQLITE3_CLOSE_OFFSET 0x1e56cd0
#define SQLITE3_STEP_OFFSET 0x1df3770
#define SQLITE3_COLUMN_COUNT_OFFSET 0x1df3c80
#define SQLITE3_COLUMN_NAME_OFFSET 0x1df4570
#define SQLITE3_COLUMN_TYPE_OFFSET 0x1df4410
#define SQLITE3_COLUMN_BLOB_OFFSET 0x1df3cc0
#define SQLITE3_COLUMN_BYTES_OFFSET 0x1df3da0
#define SQLITE3_FINALIZE_OFFSET 0x1df2740

typedef int (*Sqlite3_callback)(void*, int, char**, char**);

typedef int(__cdecl* Sqlite3_exec)(DWORD,            /* An open database */
                                   const char* sql,  /* SQL to be evaluated */
                                   Sqlite3_callback, /* Callback function */
                                   void*,        /* 1st argument to callback */
                                   char** errmsg /* Error msg written here */
);
typedef DWORD(__cdecl* Sqlite3_backup_init)(
    DWORD* pDest,           /* Destination database handle */
    const char* zDestName,  /* Destination database name */
    DWORD* pSource,         /* Source database handle */
    const char* zSourceName /* Source database name */
);
typedef int(__cdecl* Sqlite3_prepare)(
    DWORD db,          /* Database handle */
    const char* zSql,   /* SQL statement, UTF-8 encoded */
    int nByte,          /* Maximum length of zSql in bytes. */
    DWORD** ppStmt,     /* OUT: Statement handle */
    const char** pzTail /* OUT: Pointer to unused portion of zSql */
);
typedef int(__cdecl* Sqlite3_open)(const char* filename, DWORD** ppDb);
typedef int(__cdecl* Sqlite3_backup_step)(DWORD* p, int nPage);
typedef int(__cdecl* Sqlite3_backup_remaining)(DWORD* p);
typedef int(__cdecl* Sqlite3_backup_pagecount)(DWORD* p);
typedef int(__cdecl* Sqlite3_backup_finish)(DWORD* p);
typedef int(__cdecl* Sqlite3_sleep)(int);
typedef int(__cdecl* Sqlite3_errcode)(DWORD* db);
typedef int(__cdecl* Sqlite3_close)(DWORD*);

typedef int(__cdecl* Sqlite3_step)(DWORD*);
typedef int(__cdecl* Sqlite3_column_count)(DWORD* pStmt);
typedef const char*(__cdecl* Sqlite3_column_name)(DWORD*, int N);
typedef int(__cdecl* Sqlite3_column_type)(DWORD*, int iCol);
typedef const void*(__cdecl* Sqlite3_column_blob)(DWORD*, int iCol);
typedef int(__cdecl* Sqlite3_column_bytes)(DWORD*, int iCol);
typedef int(__cdecl* Sqlite3_finalize)(DWORD* pStmt);


/***************************sqlite3  end*************************************/

struct SqlResult {
  char *column_name;
  DWORD column_name_len;
  char *content;
  DWORD content_len;
  BOOL is_blob;
};

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

struct VectorInner {
#ifdef _DEBUG
  DWORD head;
#endif
  DWORD start;
  DWORD finsh;
  DWORD end;
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

struct Unkown{
  DWORD field1 = 0;
  DWORD field2= 0;
  DWORD field3= 0;
  DWORD field4= 0;
  DWORD field5= 0;
  DWORD field6= 0;
};
#endif