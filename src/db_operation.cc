#include "pch.h"
#include "db_operation.h"

#include "base64.h"
#include "common.h"
#include "new_sqlite3.h"


/// @brief free data
void FreeResult(vector<vector<SqlResult>> &data) {
  if (data.size() == 0) {
    return;
  }
  for (unsigned int i = 0; i < data.size(); i++) {
    for (unsigned j = 0; j < data[i].size(); j++) {
      SqlResult *sr = (SqlResult *)&data[i][j];
      if (sr->column_name) {
        delete sr->column_name;
        sr->column_name = NULL;
      }
      if (sr->content) {
        delete sr->content;
        sr->content = NULL;
      }
    }
    data[i].clear();
  }
  data.clear();
}

int SelectDataInner(DWORD db, const char *sql,
                    vector<vector<SqlResult>> &data) {
  DWORD wxBaseAddress = GetWeChatWinBase();
  Sqlite3_prepare p_Sqlite3_prepare =
      (Sqlite3_prepare)(wxBaseAddress + SQLITE3_PREPARE_OFFSET);
  Sqlite3_step p_Sqlite3_step =
      (Sqlite3_step)(wxBaseAddress + SQLITE3_STEP_OFFSET);
  Sqlite3_column_count p_Sqlite3_column_count =
      (Sqlite3_column_count)(wxBaseAddress + SQLITE3_COLUMN_COUNT_OFFSET);
  Sqlite3_column_name p_Sqlite3_column_name =
      (Sqlite3_column_name)(wxBaseAddress + SQLITE3_COLUMN_NAME_OFFSET);
  Sqlite3_column_type p_Sqlite3_column_type =
      (Sqlite3_column_type)(wxBaseAddress + SQLITE3_COLUMN_TYPE_OFFSET);
  Sqlite3_column_blob p_Sqlite3_column_blob =
      (Sqlite3_column_blob)(wxBaseAddress + SQLITE3_COLUMN_BLOB_OFFSET);
  Sqlite3_column_bytes p_Sqlite3_column_bytes =
      (Sqlite3_column_bytes)(wxBaseAddress + SQLITE3_COLUMN_BYTES_OFFSET);
  Sqlite3_finalize p_Sqlite3_finalize =
      (Sqlite3_finalize)(wxBaseAddress + SQLITE3_FINALIZE_OFFSET);
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

int Select(DWORD db_hanle, const char *sql,
           vector<vector<string>> &query_result) {
  vector<vector<SqlResult>> data;
  int status = SelectDataInner(db_hanle, sql, data);
  if (status == 0) {
    return 0;
  }
  vector<string> index;
  for (size_t i = 0; i < data[0].size(); i++)
    index.push_back(data[0][i].column_name);
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

/// @brief  sqlite3_exec
/// @param db
/// @param sql
/// @param callback
/// @param data
/// @return
int ExecuteSQL(DWORD db, const char *sql, DWORD callback, void *data) {
  DWORD base = GetWeChatWinBase();
  DWORD sqlite3_exec_addr = base + SQLITE3_EXEC_OFFSET;
  Sqlite3_exec fn_sqlite3_exec = (Sqlite3_exec)sqlite3_exec_addr;
  int status = fn_sqlite3_exec(db, sql, (Sqlite3_callback)callback, data, 0);
  return status;
}