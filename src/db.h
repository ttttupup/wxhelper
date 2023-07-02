#ifndef WXHELPER_DB_H_
#define WXHELPER_DB_H_
#include <string>
#include <vector>

#include "wechat_function.h"
#include "windows.h"
#include "singleton.h"
namespace wxhelper {
class DB :public Singleton<DB>{
 public:
  void init(UINT64 base);
  int ExecuteSQL(UINT64 db, const char *sql, UINT64 callback, void *data);

  int Select(UINT64 db_hanle, const char *sql,
             std::vector<std::vector<std::string>> &query_result);

  std::vector<void *> GetDbHandles();
  UINT64 GetDbHandleByDbName(wchar_t *dbname);
  INT64 GetLocalIdByMsgId(ULONG64 msgid, INT64 &dbIndex);
  std::vector<std::string> GetChatMsgByMsgId(ULONG64 msgid);

  std::string GetVoiceBuffByMsgId(ULONG64 msgid);

  std::string GetPublicMsgCompressContentByMsgId(ULONG64 msgid);

 private:
  int SelectDataInner(UINT64 db, const char *sql,
                      std::vector<std::vector<common::SqlResult>> &data);

 private:
  std::map<std::wstring, common::DatabaseInfo> dbmap_;
  std::vector<common::DatabaseInfo> dbs_;
  UINT64 base_addr_;
};

}  // namespace wxhelper

#endif