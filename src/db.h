#ifndef WXHELPER_DB_H_
#define WXHELPER_DB_H_
#include <string>
#include <vector>

#include "base_mgr.h"
#include "wechat_function.h"
#include "windows.h"
#include "singleton.h"
namespace wxhelper {
class DB :public Singleton<DB>{
 public:
  void init(DWORD base);
  int ExecuteSQL(DWORD db, const char *sql, DWORD callback, void *data);

  int Select(DWORD db_hanle, const char *sql,
             std::vector<std::vector<std::string>> &query_result);

  std::vector<void *> GetDbHandles();
  DWORD GetDbHandleByDbName(wchar_t *dbname);
  unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex);
  std::vector<std::string> GetChatMsgByMsgId(ULONG64 msgid);

  std::string GetVoiceBuffByMsgId(ULONG64 msgid);

 private:
  int SelectDataInner(DWORD db, const char *sql,
                      std::vector<std::vector<SqlResult>> &data);

 private:
  std::map<std::wstring, DatabaseInfo> dbmap_;
  std::vector<DatabaseInfo> dbs_;
  DWORD base_addr_;
};

}  // namespace wxhelper

#endif