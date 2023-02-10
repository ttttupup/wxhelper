#include "get_db_handle.h"

#include "common.h"
#include "db_operation.h"
#include "new_sqlite3.h"
#include "pch.h"
#include "wechat_data.h"
#define CONTACT_G_PINSTANCE_OFFSET 0x2e2d628
#define DB_MICRO_MSG_OFFSET 0x68
#define DB_CHAT_MSG_OFFSET 0x1C0
#define DB_MISC_OFFSET 0x3D8
#define DB_EMOTION_OFFSET 0x558
#define DB_MEDIA_OFFSET 0x9B8
#define DB_BIZCHAT_MSG_OFFSET 0x1120
#define DB_FUNCTION_MSG_OFFSET 0x11B0
#define DB_NAME_OFFSET 0x14

#define PUBLIC_MSG_MGR_OFFSET  0x2e6ce20
#define MULTI_DB_MSG_MGR_OFFSET  0x2e6ec84
#define FAVORITE_STORAGE_MGR_OFFSET  0x2e6e630
#define FTS_FAVORITE_MGR_OFFSET  0x2e2e168

using namespace std;
map<wstring, DatabaseInfo> dbmap;
std::vector<DatabaseInfo> dbs;

int GetDbInfo(void *data, int argc, char **argv, char **name) {
  DatabaseInfo *pdata = (DatabaseInfo *)data;
  TableInfo tb = {0};
  if (argv[1]) {
    tb.name = new char[strlen(argv[1]) + 1];
    memcpy(tb.name, argv[1], strlen(argv[1]) + 1);
  } else {
    tb.name = (char *)"NULL";
  }
  if (argv[2]) {
    tb.table_name = new char[strlen(argv[2]) + 1];
    memcpy(tb.table_name, argv[2], strlen(argv[2]) + 1);
  } else {
    tb.table_name = (char *)"NULL";
  }
  if (argv[3]) {
    tb.rootpage = new char[strlen(argv[3]) + 1];
    memcpy(tb.rootpage, argv[3], strlen(argv[3]) + 1);
  } else {
    tb.rootpage = (char *)"NULL";
  }
  if (argv[4]) {
    tb.sql = new char[strlen(argv[4]) + 1];
    memcpy(tb.sql, argv[4], strlen(argv[4]) + 1);
  } else {
    tb.sql = (char *)"NULL";
  }
  tb.name_len = strlen(tb.name);
  tb.table_name_len = strlen(tb.table_name);
  tb.sql_len = strlen(tb.sql);
  tb.rootpage_len = strlen(tb.rootpage);
  pdata->tables.push_back(tb);
  pdata->count = pdata->tables.size();
  return 0;
}

std::vector<void *> GetDbHandles() {
  dbs.clear();
  dbmap.clear();
  DWORD base = GetWeChatWinBase();
  DWORD p_contact_addr = *(DWORD *)(base + CONTACT_G_PINSTANCE_OFFSET);
  DWORD micro_msg_db_addr = *(DWORD *)(p_contact_addr + DB_MICRO_MSG_OFFSET);
  DWORD chat_msg_db_addr = *(DWORD *)(p_contact_addr + DB_CHAT_MSG_OFFSET);
  DWORD misc_db_addr = *(DWORD *)(p_contact_addr + DB_MISC_OFFSET);
  DWORD emotion_db_addr = *(DWORD *)(p_contact_addr + DB_EMOTION_OFFSET);
  DWORD media_db_addr = *(DWORD *)(p_contact_addr + DB_MEDIA_OFFSET);
  DWORD bizchat_msg_db_addr =
      *(DWORD *)(p_contact_addr + DB_BIZCHAT_MSG_OFFSET);
  DWORD function_msg_db_addr =
      *(DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET);

  // microMsg.db
  DatabaseInfo micro_msg_db{0};
  micro_msg_db.db_name = (wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_MICRO_MSG_OFFSET + DB_NAME_OFFSET));
  micro_msg_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_MICRO_MSG_OFFSET + DB_NAME_OFFSET + 0x4);
  micro_msg_db.handle = micro_msg_db_addr;
  ExecuteSQL(micro_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &micro_msg_db);
  dbs.push_back(micro_msg_db);
  wstring micro_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_MICRO_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap[micro_msg_name] = micro_msg_db;

  // chatMsg.db
  DatabaseInfo chat_msg_db{0};
  chat_msg_db.db_name = (wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_CHAT_MSG_OFFSET + DB_NAME_OFFSET));
  chat_msg_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_CHAT_MSG_OFFSET + DB_NAME_OFFSET + 0x4);
  chat_msg_db.handle = chat_msg_db_addr;
  ExecuteSQL(chat_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &chat_msg_db);
  dbs.push_back(chat_msg_db);
  wstring chat_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_CHAT_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap[chat_msg_name] = chat_msg_db;

  // misc.db
  DatabaseInfo misc_db{0};
  misc_db.db_name =
      (wchar_t *)(*(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET));
  misc_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET + 0x4);
  misc_db.handle = misc_db_addr;
  ExecuteSQL(misc_db_addr, "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &misc_db);
  dbs.push_back(misc_db);
  wstring misc_name = wstring((
      wchar_t *)(*(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET)));
  dbmap[misc_name] = misc_db;

  // emotion.db
  DatabaseInfo emotion_db{0};
  emotion_db.db_name = (wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_EMOTION_OFFSET + DB_NAME_OFFSET));
  emotion_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_EMOTION_OFFSET + DB_NAME_OFFSET + 0x4);
  emotion_db.handle = emotion_db_addr;
  ExecuteSQL(emotion_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &emotion_db);
  dbs.push_back(emotion_db);
  wstring emotion_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_EMOTION_OFFSET + DB_NAME_OFFSET)));
  dbmap[emotion_name] = emotion_db;

  // media.db
  DatabaseInfo media_db{0};
  media_db.db_name = (wchar_t *)(*(DWORD *)(p_contact_addr + DB_MEDIA_OFFSET +
                                            DB_NAME_OFFSET));
  media_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_MEDIA_OFFSET + DB_NAME_OFFSET + 0x4);
  media_db.handle = media_db_addr;
  ExecuteSQL(media_db_addr, "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &media_db);
  dbs.push_back(media_db);
  wstring media_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_MEDIA_OFFSET + DB_NAME_OFFSET)));
  dbmap[media_name] = media_db;

  // functionMsg.db
  DatabaseInfo function_msg_db{0};
  function_msg_db.db_name = (wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET));
  function_msg_db.db_name_len = *(
      DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET + 0x4);
  function_msg_db.handle = function_msg_db_addr;
  ExecuteSQL(function_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &function_msg_db);
  dbs.push_back(function_msg_db);
  wstring function_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap[function_msg_name] = function_msg_db;

  if (bizchat_msg_db_addr) {
    // functionMsg.db   maybe null
    DatabaseInfo bizchat_msg_db{0};
    bizchat_msg_db.db_name = (wchar_t *)(*(
        DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET));
    bizchat_msg_db.db_name_len =
        *(DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET +
                   0x4);
    bizchat_msg_db.handle = bizchat_msg_db_addr;
    ExecuteSQL(bizchat_msg_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (DWORD)GetDbInfo, &bizchat_msg_db);
    dbs.push_back(bizchat_msg_db);
    wstring bizchat_msg_name = wstring((wchar_t *)(*(
        DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET)));
    dbmap[bizchat_msg_name] = bizchat_msg_db;
  }
  DWORD multi_db_mgr_addr = base + MULTI_DB_MSG_MGR_OFFSET;
  DWORD public_msg_mgr_addr = base + PUBLIC_MSG_MGR_OFFSET;
  DWORD favorite_storage_mgr_addr = base + FAVORITE_STORAGE_MGR_OFFSET;
  DWORD fts_favorite_mgr_addr = base + FTS_FAVORITE_MGR_OFFSET;

  // MsgX.db
  DWORD wrap_ptr = *(DWORD *)(multi_db_mgr_addr);
  DWORD db_num = *(DWORD *)(wrap_ptr+0x30);
  DWORD current_db_num = *(DWORD *)(wrap_ptr+0x38);
  DWORD begin_ptr = *(DWORD *)(wrap_ptr + 0x2c);

  for (unsigned int i = 0; i < current_db_num; i++) {
    DWORD next_addr = begin_ptr + i * 0x4;
    DWORD db_addr = *(DWORD *) next_addr;
    if (db_addr) {
      DWORD msg0_db_addr = *(DWORD *)(db_addr + 0x60);
      DatabaseInfo msg0_db{0};
      msg0_db.db_name = (wchar_t *)(*(DWORD *)(db_addr));
      msg0_db.db_name_len = *(DWORD *)(db_addr + 0x4);
      msg0_db.handle = msg0_db_addr;
      msg0_db.extrainfo = *(DWORD *) (*(DWORD *)(db_addr + 0x18) +0x144);
       ExecuteSQL(
          msg0_db_addr, "select * from sqlite_master where type=\"table\";",
          (DWORD)GetDbInfo, &msg0_db);
      dbs.push_back(msg0_db);
      wstring msg_db_name = wstring((wchar_t *)(*(DWORD *)(db_addr)));
      dbmap[msg_db_name] = msg0_db;
    }
  }

  // publicMsg.db
  DWORD public_msg_ptr = *(DWORD *) (*(DWORD *)(public_msg_mgr_addr) + 0x8);
  if (public_msg_ptr) {
    DWORD public_msg_db_addr = *(DWORD *)(public_msg_ptr + 0x38);
    DatabaseInfo public_msg_db{0};
    public_msg_db.db_name = (wchar_t *)(*(DWORD *)(public_msg_ptr + 0x4C));
    public_msg_db.db_name_len =*(DWORD *)(public_msg_ptr + 0x50);
    public_msg_db.handle = public_msg_db_addr;
    ExecuteSQL(public_msg_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (DWORD)GetDbInfo, &public_msg_db);
    dbs.push_back(public_msg_db);
    wstring public_msg_db_name =wstring((wchar_t *)(*(DWORD *)(public_msg_ptr + 0x4C)));
    dbmap[public_msg_db_name] = public_msg_db;
  }

  // Favorite.db
  DWORD favItems_ptr = *(DWORD *)(*(DWORD *)(favorite_storage_mgr_addr) + 0x8);
  if (favItems_ptr) {
    DWORD favorite_db_addr = *(DWORD *)(favItems_ptr + 0x38);
    DatabaseInfo favorite_db{0};
    favorite_db.db_name =(wchar_t *)(*(DWORD *)(favItems_ptr + 0x4C));
    favorite_db.db_name_len = *(DWORD *)(favItems_ptr + 0x50);
    favorite_db.handle = favorite_db_addr;
    ExecuteSQL(favorite_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (DWORD)GetDbInfo, &favorite_db);
    dbs.push_back(favorite_db);
    wstring public_msg_db_name =wstring((wchar_t *)(*(DWORD *)(favItems_ptr + 0x4C)));
    dbmap[public_msg_db_name] = favorite_db;
  }

  DatabaseInfo db_end = {0};
  dbs.push_back(db_end);
#ifdef _DEBUG
  for (unsigned int i = 0; i < dbs.size() - 1; i++) {
    printf("dbname = %ws,handle = 0x%08X,table_count:%d\n", dbs[i].db_name,
           dbs[i].handle, dbs[i].tables.size());
    for (unsigned int j = 0; j < dbs[i].tables.size(); j++) {
      cout << "name     = " << dbs[i].tables[j].name << endl;
      cout << "tbl_name = " << dbs[i].tables[j].table_name << endl;
      cout << "rootpage = " << dbs[i].tables[j].rootpage << endl;
      cout << "sql      = " << dbs[i].tables[j].sql << endl;
      cout << endl;
    }
    cout << endl;
  }
#endif
  vector<void *> ret_array;
  for (unsigned int i = 0; i < dbs.size() - 1; i++)
    ret_array.push_back(&dbs[i]);
  return ret_array;
}

DWORD GetDbHandleByDbName(wchar_t *dbname) {
  if (dbmap.size() == 0) {
    GetDbHandles();
  }
  if (dbmap.find(dbname) != dbmap.end()) {
    return dbmap[dbname].handle;
  }

  return 0;
}

unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex) {
  char sql[260] = {0};
  sprintf_s(sql, "select localId from MSG where MsgSvrID=%llu;", msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MSG%d.db", i);
    DWORD handle = GetDbHandleByDbName(dbname);
    #ifdef _DEBUG
    cout <<" handle =" <<handle<<endl;
    #endif
    if (handle == 0) return 0;
    vector<vector<string>> result;
    int ret = Select(handle, (const char *)sql, result);
    #ifdef _DEBUG
    cout <<" size =" <<result.size()<<endl;
    #endif
    if (result.size() == 0) continue;
    dbIndex = dbmap[dbname].extrainfo;
    return stoi(result[1][0]);
  }
  return 0;
}

vector<string> GetChatMsgByMsgId(ULONG64 msgid){
  char sql[260] = {0};
  sprintf_s(sql, "select localId,TalkerId,MsgSvrID,Type,SubType,IsSender,CreateTime,Sequence,StatusEx,FlagEx,Status,MsgServerSeq,MsgSequence,StrTalker,StrContent,BytesExtra from MSG where MsgSvrID=%llu;", msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MSG%d.db", i);
    DWORD handle = GetDbHandleByDbName(dbname);
    if (handle == 0) return {};
    vector<vector<string>> result;
    int ret = Select(handle, (const char *)sql, result);
    #ifdef _DEBUG
    cout <<" size =" <<result.size()<<endl;
    #endif
    if (result.size() == 0) continue;
    return result[1];
  }
  return {};
}