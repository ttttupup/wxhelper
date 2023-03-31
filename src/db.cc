#include "pch.h"
#include "db.h"

#include "base64.h"
#include "easylogging++.h"

#include "wechat_function.h"
using namespace std;

namespace wxhelper {

void DB::init(DWORD base) {
  base_addr_ = base;
  dbmap_ = {};
  dbs_ = {};
}

void FreeResult(vector<vector<SqlResult>> &data) {
  if (data.size() == 0) {
    return;
  }
  for (unsigned int i = 0; i < data.size(); i++) {
    for (unsigned j = 0; j < data[i].size(); j++) {
      SqlResult *sr = (SqlResult *)&data[i][j];
      if (sr->column_name) {
        delete[] sr->column_name;
        sr->column_name = NULL;
      }
      if (sr->content) {
        delete[] sr->content;
        sr->content = NULL;
      }
    }
    data[i].clear();
  }
  data.clear();
}

int DB::SelectDataInner(DWORD db, const char *sql,
                           vector<vector<SqlResult>> &data) {
  Sqlite3_prepare p_Sqlite3_prepare =
      (Sqlite3_prepare)(base_addr_ + SQLITE3_PREPARE_OFFSET);
  Sqlite3_step p_Sqlite3_step =
      (Sqlite3_step)(base_addr_ + SQLITE3_STEP_OFFSET);
  Sqlite3_column_count p_Sqlite3_column_count =
      (Sqlite3_column_count)(base_addr_ + SQLITE3_COLUMN_COUNT_OFFSET);
  Sqlite3_column_name p_Sqlite3_column_name =
      (Sqlite3_column_name)(base_addr_ + SQLITE3_COLUMN_NAME_OFFSET);
  Sqlite3_column_type p_Sqlite3_column_type =
      (Sqlite3_column_type)(base_addr_ + SQLITE3_COLUMN_TYPE_OFFSET);
  Sqlite3_column_blob p_Sqlite3_column_blob =
      (Sqlite3_column_blob)(base_addr_ + SQLITE3_COLUMN_BLOB_OFFSET);
  Sqlite3_column_bytes p_Sqlite3_column_bytes =
      (Sqlite3_column_bytes)(base_addr_ + SQLITE3_COLUMN_BYTES_OFFSET);
  Sqlite3_finalize p_Sqlite3_finalize =
      (Sqlite3_finalize)(base_addr_ + SQLITE3_FINALIZE_OFFSET);
  DWORD *stmt;
  int rc = p_Sqlite3_prepare(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) return NULL;
  while (p_Sqlite3_step(stmt) == SQLITE_ROW) {
    int col_count = p_Sqlite3_column_count(stmt);
    vector<SqlResult> tempStruct;
    for (int i = 0; i < col_count; i++) {
      SqlResult temp = {0};
      const char *ColName = p_Sqlite3_column_name(stmt, i);
      int nType = p_Sqlite3_column_type(stmt, i);
      const void *pReadBlobData = p_Sqlite3_column_blob(stmt, i);
      int nLength = p_Sqlite3_column_bytes(stmt, i);
      temp.column_name = new char[strlen(ColName) + 1];
      memcpy(temp.column_name, ColName, strlen(ColName) + 1);
      temp.column_name_len = strlen(ColName);
      temp.content_len = nLength;
      switch (nType) {
        case SQLITE_BLOB: {
          temp.content = new char[nLength];
          memcpy(temp.content, pReadBlobData, nLength);
          temp.is_blob = true;
          break;
        }
        default: {
          if (nLength != 0) {
            temp.content = new char[nLength + 1];
            memcpy(temp.content, pReadBlobData, nLength + 1);
          } else {
            temp.content = new char[2];
            ZeroMemory(temp.content, 2);
          }
          temp.is_blob = false;
          break;
        }
      }
      tempStruct.push_back(temp);
    }
    data.push_back(tempStruct);
  }
  p_Sqlite3_finalize(stmt);
  return 1;
}

int DB::Select(DWORD db_hanle, const char *sql,
                  vector<vector<string>> &query_result) {
  vector<vector<SqlResult>> data;
  int status = SelectDataInner(db_hanle, sql, data);
  if (status == 0) {
    return 0;
  }
  if (data.size() == 0) {
    return 1;
  }
  vector<string> index;
  for (size_t i = 0; i < data[0].size(); i++) {
    index.push_back(data[0][i].column_name);
  }
  query_result.push_back(index);

  for (auto it : data) {
    vector<string> item;
    for (size_t i = 0; i < it.size(); i++) {
      if (!it[i].is_blob) {
        string content(it[i].content);
        item.push_back(content);
      } else {
        string b64_str =
            base64_encode((BYTE *)it[i].content, it[i].content_len);
        item.push_back(b64_str);
      }
    }
    query_result.push_back(item);
  }
  FreeResult(data);
  return 1;
}

int SelectDbInfo(void *data, int argc, char **argv, char **name) {
  vector<SqlResult> result;
  for (int i = 0; i < argc; i++) {
    SqlResult temp = {0};
    temp.column_name = new char[strlen(name[i]) + 1];
    memcpy(temp.column_name, name[i], strlen(name[i]) + 1);
    temp.column_name_len = strlen(name[i]);
    if (argv[i]) {
      temp.content = new char[strlen(argv[i]) + 1];
      memcpy(temp.content, argv[i], strlen(argv[i]) + 1);
      temp.content_len = strlen(argv[i]);
    } else {
      temp.content = new char[2];
      ZeroMemory(temp.content, 2);
      temp.content_len = 0;
    }
    result.push_back(temp);
  }
  return 1;
}

int DB::ExecuteSQL(DWORD db, const char *sql, DWORD callback, void *data) {
  DWORD sqlite3_exec_addr = base_addr_ + SQLITE3_EXEC_OFFSET;
  Sqlite3_exec fn_sqlite3_exec = (Sqlite3_exec)sqlite3_exec_addr;
  int status = fn_sqlite3_exec(db, sql, (Sqlite3_callback)callback, data, 0);
  return status;
}

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

std::vector<void *> DB::GetDbHandles() {
  dbs_.clear();
  dbmap_.clear();
  DWORD p_contact_addr = *(DWORD *)(base_addr_ + CONTACT_G_PINSTANCE_OFFSET);
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
  dbs_.push_back(micro_msg_db);
  wstring micro_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_MICRO_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap_[micro_msg_name] = micro_msg_db;

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
  dbs_.push_back(chat_msg_db);
  wstring chat_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_CHAT_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap_[chat_msg_name] = chat_msg_db;

  // misc.db
  DatabaseInfo misc_db{0};
  misc_db.db_name =
      (wchar_t *)(*(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET));
  misc_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET + 0x4);
  misc_db.handle = misc_db_addr;
  ExecuteSQL(misc_db_addr, "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &misc_db);
  dbs_.push_back(misc_db);
  wstring misc_name = wstring((
      wchar_t *)(*(DWORD *)(p_contact_addr + DB_MISC_OFFSET + DB_NAME_OFFSET)));
  dbmap_[misc_name] = misc_db;

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
  dbs_.push_back(emotion_db);
  wstring emotion_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_EMOTION_OFFSET + DB_NAME_OFFSET)));
  dbmap_[emotion_name] = emotion_db;

  // media.db
  DatabaseInfo media_db{0};
  media_db.db_name = (wchar_t *)(*(DWORD *)(p_contact_addr + DB_MEDIA_OFFSET +
                                            DB_NAME_OFFSET));
  media_db.db_name_len =
      *(DWORD *)(p_contact_addr + DB_MEDIA_OFFSET + DB_NAME_OFFSET + 0x4);
  media_db.handle = media_db_addr;
  ExecuteSQL(media_db_addr, "select * from sqlite_master where type=\"table\";",
             (DWORD)GetDbInfo, &media_db);
  dbs_.push_back(media_db);
  wstring media_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_MEDIA_OFFSET + DB_NAME_OFFSET)));
  dbmap_[media_name] = media_db;

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
  dbs_.push_back(function_msg_db);
  wstring function_msg_name = wstring((wchar_t *)(*(
      DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET)));
  dbmap_[function_msg_name] = function_msg_db;

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
    dbs_.push_back(bizchat_msg_db);
    wstring bizchat_msg_name = wstring((wchar_t *)(*(
        DWORD *)(p_contact_addr + DB_FUNCTION_MSG_OFFSET + DB_NAME_OFFSET)));
    dbmap_[bizchat_msg_name] = bizchat_msg_db;
  }
  // Storage
  DWORD storage_start = *(DWORD *)(p_contact_addr + STORAGE_START_OFFSET);
  DWORD storage_end = *(DWORD *)(p_contact_addr + STORAGE_END_OFFSET);

  // do {
  //   DWORD vtable_ptr = *(DWORD *)(storage_start);

  //   if(vtable_ptr == base + OP_LOG_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + CHAT_MSG_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + CHAT_CR_MSG_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + SESSION_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + APP_INFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + HEAD_IMG_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + HEAD_IMG_URL_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + BIZ_INFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + TICKET_INFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + CHAT_ROOM_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + CHAT_ROOM_INFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + MEDIA_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + NAME_2_ID_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + EMOTION_PACKAGE_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + EMOTION_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + BUFINFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + CUSTOM_EMOTION_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + DEL_SESSIONINFO_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + FUNCTION_MSG_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + FUNCTION_MSG_TASK_STORAGE_VFTABLE){

  //   }else if(vtable_ptr == base + REVOKE_MSG_STORAGE_VFTABLE){

  //   }

  //   storage_start = storage_start + 0x4;
  // } while (storage_start != storage_end);

  DWORD multi_db_mgr_addr = base_addr_ + MULTI_DB_MSG_MGR_OFFSET;
  DWORD public_msg_mgr_addr = base_addr_ + PUBLIC_MSG_MGR_OFFSET;
  DWORD favorite_storage_mgr_addr = base_addr_ + FAVORITE_STORAGE_MGR_OFFSET;
  DWORD fts_favorite_mgr_addr = base_addr_ + FTS_FAVORITE_MGR_OFFSET;

  // MsgX.db
  DWORD wrap_ptr = *(DWORD *)(multi_db_mgr_addr);
  DWORD db_num = *(DWORD *)(wrap_ptr + 0x30);
  DWORD current_db_num = *(DWORD *)(wrap_ptr + 0x38);
  DWORD begin_ptr = *(DWORD *)(wrap_ptr + 0x2c);

  for (unsigned int i = 0; i < current_db_num; i++) {
    DWORD next_addr = begin_ptr + i * 0x4;
    DWORD db_addr = *(DWORD *)next_addr;
    if (db_addr) {
      DWORD msg0_db_addr = *(DWORD *)(db_addr + 0x60);
      DatabaseInfo msg0_db{0};
      msg0_db.db_name = (wchar_t *)(*(DWORD *)(db_addr));
      msg0_db.db_name_len = *(DWORD *)(db_addr + 0x4);
      msg0_db.handle = msg0_db_addr;
      msg0_db.extrainfo = *(DWORD *)(*(DWORD *)(db_addr + 0x18) + 0x144);
      ExecuteSQL(msg0_db_addr,
                 "select * from sqlite_master where type=\"table\";",
                 (DWORD)GetDbInfo, &msg0_db);
      dbs_.push_back(msg0_db);
      wstring msg_db_name = wstring((wchar_t *)(*(DWORD *)(db_addr)));
      dbmap_[msg_db_name] = msg0_db;

      // BufInfoStorage
      DWORD buf_info_addr = *(DWORD *)(db_addr + 0x14);

      DWORD buf_info_handle = *(DWORD *)(buf_info_addr + 0x38);
      DatabaseInfo media_msg0_db{0};
      media_msg0_db.db_name = (wchar_t *)(*(DWORD *)(buf_info_addr + 0x4C));
      media_msg0_db.db_name_len = *(DWORD *)(buf_info_addr + 0x50);
      media_msg0_db.handle = buf_info_handle;
      ExecuteSQL(buf_info_handle,
                 "select * from sqlite_master where type=\"table\";",
                 (DWORD)GetDbInfo, &media_msg0_db);
      dbs_.push_back(media_msg0_db);
      wstring media_msg_db_name =
          wstring((wchar_t *)(*(DWORD *)(buf_info_addr + 0x4C)));
      dbmap_[media_msg_db_name] = media_msg0_db;
    }
  }

  // publicMsg.db
  DWORD public_msg_ptr = *(DWORD *)(*(DWORD *)(public_msg_mgr_addr) + 0x8);
  if (public_msg_ptr) {
    DWORD public_msg_db_addr = *(DWORD *)(public_msg_ptr + 0x38);
    DatabaseInfo public_msg_db{0};
    public_msg_db.db_name = (wchar_t *)(*(DWORD *)(public_msg_ptr + 0x4C));
    public_msg_db.db_name_len = *(DWORD *)(public_msg_ptr + 0x50);
    public_msg_db.handle = public_msg_db_addr;
    ExecuteSQL(public_msg_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (DWORD)GetDbInfo, &public_msg_db);
    dbs_.push_back(public_msg_db);
    wstring public_msg_db_name =
        wstring((wchar_t *)(*(DWORD *)(public_msg_ptr + 0x4C)));
    dbmap_[public_msg_db_name] = public_msg_db;
  }

  // Favorite.db
  DWORD favItems_ptr =
      *(DWORD *)(*(DWORD *)(*(DWORD *)(favorite_storage_mgr_addr) + 0x8) + 0x4);
  if (favItems_ptr) {
    DWORD favorite_db_addr = *(DWORD *)(favItems_ptr + 0x38);
    DatabaseInfo favorite_db{0};
    favorite_db.db_name = (wchar_t *)(*(DWORD *)(favItems_ptr + 0x4C));
    favorite_db.db_name_len = *(DWORD *)(favItems_ptr + 0x50);
    favorite_db.handle = favorite_db_addr;
    ExecuteSQL(favorite_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (DWORD)GetDbInfo, &favorite_db);
    dbs_.push_back(favorite_db);
    wstring public_msg_db_name =
        wstring((wchar_t *)(*(DWORD *)(favItems_ptr + 0x4C)));
    dbmap_[public_msg_db_name] = favorite_db;
  }

  DatabaseInfo db_end = {0};
  dbs_.push_back(db_end);
#ifdef _DEBUG
  for (unsigned int i = 0; i < dbs_.size() - 1; i++) {
    LOG(INFO) << "dbname =" << dbs_[i].db_name;
    LOG(INFO) << "handle =" << dbs_[i].handle;
    LOG(INFO) << "table_count =" << dbs_[i].tables.size();
  }
#endif
  vector<void *> ret_array;
  for (unsigned int i = 0; i < dbs_.size() - 1; i++)
    ret_array.push_back(&dbs_[i]);
  return ret_array;
}

DWORD DB::GetDbHandleByDbName(wchar_t *dbname) {
  if (dbmap_.size() == 0) {
    GetDbHandles();
  }
  if (dbmap_.find(dbname) != dbmap_.end()) {
    return dbmap_[dbname].handle;
  }

  return 0;
}

unsigned int DB::GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex) {
  char sql[260] = {0};
  sprintf_s(sql, "select localId from MSG where MsgSvrID=%llu;", msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MSG%d.db", i);
    DWORD handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      LOG(INFO) << "MSG db handle is null";
      return 0;
    }
    vector<vector<string>> result;
    int ret = Select(handle, (const char *)sql, result);
    if (result.size() == 0) continue;
    dbIndex = dbmap_[dbname].extrainfo;
    return stoi(result[1][0]);
  }
  return 0;
}

vector<string> DB::GetChatMsgByMsgId(ULONG64 msgid) {
  char sql[260] = {0};
  sprintf_s(sql,
            "select "
            "localId,TalkerId,MsgSvrID,Type,SubType,IsSender,CreateTime,"
            "Sequence,StatusEx,FlagEx,Status,MsgServerSeq,MsgSequence,"
            "StrTalker,StrContent,BytesExtra from MSG where MsgSvrID=%llu;",
            msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MSG%d.db", i);
    DWORD handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      LOG(INFO) << "MSG db handle is null";
      return {};
    }
    vector<vector<string>> result;
    int ret = Select(handle, (const char *)sql, result);
    if (result.size() == 0) continue;
    return result[1];
  }
  return {};
}

std::string DB::GetVoiceBuffByMsgId(ULONG64 msgid) {
  char sql[260] = {0};
  sprintf_s(sql, "SELECT Buf from Media  WHERE Reserved0=%llu;", msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MediaMSG%d.db", i);
    DWORD handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      LOG(INFO) << "Media db handle is null";
      return "";
    }
    vector<vector<string>> result;
    int ret = Select(handle, (const char *)sql, result);
    if (result.size() == 0) continue;
    return result[1][0];
  }
  return "";
}
}  // namespace wxhelper