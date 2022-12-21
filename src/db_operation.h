#ifndef DB_OPERATION_H_
#define DB_OPERATION_H_
#include <windows.h>
#include <vector>
using namespace std;
struct SqlResult {
  char *column_name;
  DWORD column_name_len;
  char *content;
  DWORD content_len;
  BOOL is_blob;
};
/// @brief exec sql
/// @param db  opened db
/// @param sql  sql
/// @param callback  callback func 
/// @param data   data
/// @return 
int ExecuteSQL(DWORD db, const char *sql, DWORD callback, void *data);

int Select(DWORD db_hanle, const char *sql,vector<vector<string>> &query_result);

#endif