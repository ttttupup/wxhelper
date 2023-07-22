#include "pch.h"
#include "db.h"

#include "base64.h"
#include "wechat_function.h"
#include "utils.h"

namespace offset = wxhelper::V3_9_5_81::offset;


namespace wxhelper {

void DB::init(UINT64 base) {
  base_addr_ = base;
  dbmap_ = {};
  dbs_ = {};
}

void FreeResult(std::vector<std::vector<common::SqlResult>> &data) {
  if (data.size() == 0) {
    return;
  }
  for (unsigned int i = 0; i < data.size(); i++) {
    for (unsigned j = 0; j < data[i].size(); j++) {
      common::SqlResult *sr = (common::SqlResult *)&data[i][j];
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

int DB::SelectDataInner(UINT64 db, const char *sql,
                           std::vector<std::vector<common::SqlResult>> &data) {
 common::sqlite3_prepare p_sqlite3_prepare =
      (common::sqlite3_prepare)(base_addr_ + offset::k_sqlite3_prepare);
  common::sqlite3_step p_sqlite3_step =
      (common::sqlite3_step)(base_addr_ + offset::k_sqlite3_step);
 common::sqlite3_column_count p_sqlite3_column_count =
      (common::sqlite3_column_count)(base_addr_ + offset::k_sqlite3_column_count);
  common::sqlite3_column_name p_sqlite3_column_name =
      (common::sqlite3_column_name)(base_addr_ + offset::k_sqlite3_column_name);
  common::sqlite3_column_type p_sqlite3_column_type =
      (common::sqlite3_column_type)(base_addr_ + offset::k_sqlite3_column_type);
 common::sqlite3_column_blob p_sqlite3_column_blob =
      (common::sqlite3_column_blob)(base_addr_ + offset::k_sqlite3_column_blob);
  common::sqlite3_column_bytes p_sqlite3_column_bytes =
      (common::sqlite3_column_bytes)(base_addr_ + offset::k_sqlite3_column_bytes);
 common::sqlite3_finalize p_sqlite3_finalize =
      (common::sqlite3_finalize)(base_addr_ + offset::k_sqlite3_finalize);
  UINT64 *stmt;
  int rc = p_sqlite3_prepare(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    return NULL;
  }
  while (p_sqlite3_step(stmt) == SQLITE_ROW) {
    int col_count = p_sqlite3_column_count(stmt);
    std::vector<common::SqlResult> tempStruct;
    for (int i = 0; i < col_count; i++) {
      common::SqlResult temp = {0};
      const char *ColName = p_sqlite3_column_name(stmt, i);
      int nType = p_sqlite3_column_type(stmt, i);
      const void *pReadBlobData = p_sqlite3_column_blob(stmt, i);
      int nLength = p_sqlite3_column_bytes(stmt, i);
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
  p_sqlite3_finalize(stmt);
  return 1;
}

int DB::Select(UINT64 db_hanle, const char *sql,
                  std::vector<std::vector<std::string>> &query_result) {
  std::vector<std::vector<common::SqlResult>> data;
  int status = SelectDataInner(db_hanle, sql, data);
  if (status == 0) {
    return 0;
  }
  if (data.size() == 0) {
    return 1;
  }
  std::vector<std::string> index;
  for (size_t i = 0; i < data[0].size(); i++) {
    index.push_back(data[0][i].column_name);
  }
  query_result.push_back(index);

  for (auto it : data) {
    std::vector<std::string> item;
    for (size_t i = 0; i < it.size(); i++) {
      if (!it[i].is_blob) {
        bool is_utf8 = Utils::IsTextUtf8(it[i].content, it[i].content_len);
        if (is_utf8) {
          std::string content(it[i].content);
          item.push_back(content);
        } else {
          std::string base64_str =
              base64_encode((BYTE *)it[i].content, it[i].content_len);
          item.push_back(base64_str);
        }

      } else {
        std::string b64_str =
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
  std::vector<common::SqlResult> result;
  for (int i = 0; i < argc; i++) {
    common::SqlResult temp = {0};
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

int DB::ExecuteSQL(UINT64 db, const char *sql, UINT64 callback, void *data) {
  UINT64 sqlite3_exec_addr = base_addr_ + offset::k_sqlite3_exec;
  common::sqlite3_exec fn_sqlite3_exec = (common::sqlite3_exec)sqlite3_exec_addr;
  int status = fn_sqlite3_exec(db, sql, (common::sqlite3_callback)callback, data, 0);
  return status;
}

int GetDbInfo(void *data, int argc, char **argv, char **name) {
  common::DatabaseInfo *pdata = (common::DatabaseInfo *)data;
  common::TableInfo tb = {0};
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
  UINT64 p_contact_addr = *(UINT64 *)(base_addr_ + offset::kGPInstance);
  UINT64 micro_msg_db_addr = *(UINT64 *)(p_contact_addr + offset::kMicroMsgDB);
  UINT64 chat_msg_db_addr = *(UINT64 *)(p_contact_addr + offset::kChatMsgDB);
  UINT64 misc_db_addr = *(UINT64 *)(p_contact_addr + offset::kMiscDB);
  UINT64 emotion_db_addr = *(UINT64 *)(p_contact_addr + offset::kEmotionDB);
  UINT64 media_db_addr = *(UINT64 *)(p_contact_addr + offset::kMediaDB);
  UINT64 bizchat_msg_db_addr =
      *(UINT64 *)(p_contact_addr + offset::kBizchatMsgDB);
  UINT64 function_msg_db_addr =
      *(UINT64 *)(p_contact_addr + offset::kFunctionMsgDB);

  // microMsg.db
  common::DatabaseInfo micro_msg_db{0};
  micro_msg_db.db_name = (wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kMicroMsgDB + offset::kDBName));
  micro_msg_db.db_name_len =
      *(DWORD *)(p_contact_addr + offset::kMicroMsgDB + offset::kDBName + 0x8);
  micro_msg_db.handle = micro_msg_db_addr;
  ExecuteSQL(micro_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &micro_msg_db);
  dbs_.push_back(micro_msg_db);
  std::wstring micro_msg_name = std::wstring((wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kMicroMsgDB + offset::kDBName)));
  dbmap_[micro_msg_name] = micro_msg_db;

  // chatMsg.db
  common::DatabaseInfo chat_msg_db{0};
  chat_msg_db.db_name = (wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kChatMsgDB + offset::kDBName));
  chat_msg_db.db_name_len =
      *(DWORD *)(p_contact_addr +  offset::kChatMsgDB + offset::kDBName + 0x8);
  chat_msg_db.handle = chat_msg_db_addr;
  ExecuteSQL(chat_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &chat_msg_db);
  dbs_.push_back(chat_msg_db);
  std::wstring chat_msg_name = std::wstring((wchar_t *)(*(
      UINT64 *)(p_contact_addr +  offset::kChatMsgDB + offset::kDBName)));
  dbmap_[chat_msg_name] = chat_msg_db;

  // misc.db
  common::DatabaseInfo misc_db{0};
  misc_db.db_name =
      (wchar_t *)(*(UINT64 *)(p_contact_addr + offset::kMiscDB + offset::kDBName));
  misc_db.db_name_len =
      *(DWORD *)(p_contact_addr + offset::kMiscDB + offset::kDBName + 0x8);
  misc_db.handle = misc_db_addr;
  ExecuteSQL(misc_db_addr, "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &misc_db);
  dbs_.push_back(misc_db);
  std::wstring misc_name = std::wstring((
      wchar_t *)(*(UINT64 *)(p_contact_addr + offset::kMiscDB + offset::kDBName)));
  dbmap_[misc_name] = misc_db;

  // emotion.db
  common::DatabaseInfo emotion_db{0};
  emotion_db.db_name = (wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kEmotionDB + offset::kDBName));
  emotion_db.db_name_len =
      *(DWORD *)(p_contact_addr + offset::kEmotionDB + offset::kDBName + 0x8);
  emotion_db.handle = emotion_db_addr;
  ExecuteSQL(emotion_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &emotion_db);
  dbs_.push_back(emotion_db);
  std::wstring emotion_name = std::wstring((wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kEmotionDB + offset::kDBName)));
  dbmap_[emotion_name] = emotion_db;

  // media.db
  common::DatabaseInfo media_db{0};
  media_db.db_name = (wchar_t *)(*(UINT64 *)(p_contact_addr + offset::kMediaDB +
                                            offset::kDBName));
  media_db.db_name_len =
      *(DWORD *)(p_contact_addr + offset::kMediaDB + offset::kDBName + 0x8);
  media_db.handle = media_db_addr;
  ExecuteSQL(media_db_addr, "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &media_db);
  dbs_.push_back(media_db);
  std::wstring media_name = std::wstring((wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kMediaDB + offset::kDBName)));
  dbmap_[media_name] = media_db;

  // functionMsg.db
  common::DatabaseInfo function_msg_db{0};
  function_msg_db.db_name = (wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName));
  function_msg_db.db_name_len = *(
      DWORD *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName + 0x8);
  function_msg_db.handle = function_msg_db_addr;
  ExecuteSQL(function_msg_db_addr,
             "select * from sqlite_master where type=\"table\";",
             (UINT64)GetDbInfo, &function_msg_db);
  dbs_.push_back(function_msg_db);
  std::wstring function_msg_name = std::wstring((wchar_t *)(*(
      UINT64 *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName)));
  dbmap_[function_msg_name] = function_msg_db;

  if (bizchat_msg_db_addr) {
    // functionMsg.db   maybe null
    common::DatabaseInfo bizchat_msg_db{0};
    bizchat_msg_db.db_name = (wchar_t *)(*(
        UINT64 *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName));
    bizchat_msg_db.db_name_len =
        *(DWORD *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName +
                   0x8);
    bizchat_msg_db.handle = bizchat_msg_db_addr;
    ExecuteSQL(bizchat_msg_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (UINT64)GetDbInfo, &bizchat_msg_db);
    dbs_.push_back(bizchat_msg_db);
    std::wstring bizchat_msg_name = std::wstring((wchar_t *)(*(
        UINT64 *)(p_contact_addr + offset::kFunctionMsgDB + offset::kDBName)));
    dbmap_[bizchat_msg_name] = bizchat_msg_db;
  }
 
  UINT64 multi_db_mgr_addr = base_addr_ + offset::kMultiDBMgr;
  UINT64 public_msg_mgr_addr = base_addr_ + offset::kPublicMsgMgr;
  UINT64 favorite_storage_mgr_addr = base_addr_ + offset::kFavoriteStorageMgr;

  // MsgX.db
  UINT64 wrap_ptr = *(UINT64 *)(multi_db_mgr_addr);
  UINT64 current_db_num = *(UINT64 *)(wrap_ptr + 0x68);
  UINT64 begin_ptr = *(UINT64 *)(wrap_ptr + 0x50);

  for (unsigned int i = 0; i < current_db_num; i++) {
    UINT64 next_addr = begin_ptr + i * 0x8;
    UINT64 db_addr = *(UINT64 *)next_addr;
    if (db_addr) {
      UINT64 msg0_db_addr = *(UINT64 *)(db_addr + 0x78);
      common::DatabaseInfo msg0_db{0};
      msg0_db.db_name = (wchar_t *)(*(UINT64 *)(db_addr));
      msg0_db.db_name_len = *(DWORD *)(db_addr + 0x8);
      msg0_db.handle = msg0_db_addr;
      msg0_db.extrainfo = *(UINT64 *)(*(UINT64 *)(db_addr + 0x28) + 0x1E8);
      ExecuteSQL(msg0_db_addr,
                 "select * from sqlite_master where type=\"table\";",
                 (UINT64)GetDbInfo, &msg0_db);
      dbs_.push_back(msg0_db);
      std::wstring msg_db_name = std::wstring(msg0_db.db_name,msg0_db.db_name_len);
      dbmap_[msg_db_name] = msg0_db;

      // BufInfoStorage
      UINT64 buf_info_addr = *(UINT64 *)(db_addr + 0x20);

      UINT64 buf_info_handle = *(UINT64 *)(buf_info_addr + 0x50);
      common::DatabaseInfo media_msg0_db{0};
      media_msg0_db.db_name = (wchar_t *)(*(UINT64 *)(buf_info_addr + 0x78));
      media_msg0_db.db_name_len = *(DWORD *)(buf_info_addr + 0x80);
      media_msg0_db.handle = buf_info_handle;
      ExecuteSQL(buf_info_handle,
                 "select * from sqlite_master where type=\"table\";",
                 (UINT64)GetDbInfo, &media_msg0_db);
      dbs_.push_back(media_msg0_db);
      std::wstring media_msg_db_name =
          std::wstring(media_msg0_db.db_name,media_msg0_db.db_name_len);
      dbmap_[media_msg_db_name] = media_msg0_db;
    }
  }


  // publicMsg.db
  UINT64 public_msg_mgr_ptr =*(UINT64 *)(public_msg_mgr_addr);
  for (unsigned int i = 1; i < 4; i++) {
    UINT64 public_msg_ptr = *(UINT64 *)(public_msg_mgr_ptr + i * 0x8);
    if (public_msg_ptr) {
      UINT64 public_msg_db_addr = *(UINT64 *)(public_msg_ptr + 0x50);
      common::DatabaseInfo public_msg_db{0};
      public_msg_db.db_name = (wchar_t *)(*(UINT64 *)(public_msg_ptr + 0x78));
      public_msg_db.db_name_len = *(DWORD *)(public_msg_ptr + 0x80);
      public_msg_db.handle = public_msg_db_addr;
      ExecuteSQL(public_msg_db_addr,
                 "select * from sqlite_master where type=\"table\";",
                 (UINT64)GetDbInfo, &public_msg_db);
      dbs_.push_back(public_msg_db);
      std::wstring public_msg_db_name =
          std::wstring(public_msg_db.db_name , public_msg_db.db_name_len);
      dbmap_[public_msg_db_name] = public_msg_db;
    }
  }

  // Favorite.db
  UINT64 favItems_ptr =
      *(UINT64 *)(*(UINT64 *)(*(UINT64 *)(favorite_storage_mgr_addr) + 0x10) + 0x8);
  if (favItems_ptr) {
    UINT64 favorite_db_addr = *(UINT64 *)(favItems_ptr + 0x50);
    common::DatabaseInfo favorite_db{0};
    favorite_db.db_name = (wchar_t *)(*(UINT64 *)(favItems_ptr + 0x78));
    favorite_db.db_name_len = *(DWORD *)(favItems_ptr + 0x80);
    favorite_db.handle = favorite_db_addr;
    ExecuteSQL(favorite_db_addr,
               "select * from sqlite_master where type=\"table\";",
               (UINT64)GetDbInfo, &favorite_db);
    dbs_.push_back(favorite_db);
    std::wstring public_msg_db_name =
        std::wstring(favorite_db.db_name,favorite_db.db_name_len);
    dbmap_[public_msg_db_name] = favorite_db;
  }
  common::DatabaseInfo db_end = {0};
  dbs_.push_back(db_end);
  std::vector<void *> ret_array;
  for (unsigned int i = 0; i < dbs_.size() - 1; i++){
    ret_array.push_back(&dbs_[i]);
  }
  return ret_array;
}

UINT64 DB::GetDbHandleByDbName(wchar_t *dbname) {
  if (dbmap_.size() == 0) {
    GetDbHandles();
  }
  if (dbmap_.find(dbname) != dbmap_.end()) {
    return dbmap_[dbname].handle;
  }

  return 0;
}

INT64 DB::GetLocalIdByMsgId(ULONG64 msgid, INT64 &dbIndex) {
  char sql[260] = {0};
  sprintf_s(sql, "select localId from MSG where MsgSvrID=%llu;", msgid);
  wchar_t dbname[20] = {0};
  for (int i = 0;; i++) {
    swprintf_s(dbname, L"MSG%d.db", i);
    UINT64 handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      SPDLOG_INFO("MSG db handle is null");
      return 0;
    }
    std::vector<std::vector<std::string>> result;
    int ret = Select(handle, (const char *)sql, result);
    if (result.size() == 0) continue;
    dbIndex = dbmap_[dbname].extrainfo;
    return stoi(result[1][0]);
  }
  return 0;
}

std::vector<std::string> DB::GetChatMsgByMsgId(ULONG64 msgid) {
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
    UINT64 handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      // LOG(INFO) << "MSG db handle is null";
      return {};
    }
    std::vector<std::vector<std::string>> result;
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
    UINT64 handle = GetDbHandleByDbName(dbname);
    if (handle == 0) {
      // LOG(INFO) << "Media db handle is null";
      return "";
    }
    std::vector<std::vector<std::string>> result;
    int ret = Select(handle, (const char *)sql, result);
    if (result.size() == 0) continue;
    return result[1][0];
  }
  return "";
}

std::string DB::GetPublicMsgCompressContentByMsgId(ULONG64 msgid) {
  char sql[260] = {0};
  sprintf_s(sql, "SELECT CompressContent from PublicMsg  WHERE MsgSvrID=%llu;", msgid);
  wchar_t dbname[20] = {0};
  swprintf_s( dbname, 20, L"%s", L"PublicMsg.db");
  UINT64 handle = GetDbHandleByDbName(dbname);
  if (handle == 0) {
    return "";
  }
  std::vector<std::vector<std::string>> result;
  int ret = Select(handle, (const char *)sql, result);
  if (result.size() == 0)  {
    return "";
  }
  return result[1][0];
}

}  // namespace wxhelper