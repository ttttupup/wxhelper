#ifndef WXHELPER_WECHAT_DB_H_
#define WXHELPER_WECHAT_DB_H_
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "singleton.h"
#include "sqlite_function.h"
#include "wechat_interface.h"

namespace wechat {

class WeChatDb : public base::Singleton<WeChatDb> {
 public:
  void Init();
  std::vector<void *> GetWeChatDbHandles();
  int Select(uint64_t db_hanle, const std::string &sql,
             std::vector<std::vector<std::string>> &query_result);

  int64_t GetLocalIdByMsgId(uint64_t msgid, int64_t &dbIndex);
  std::vector<std::string> GetChatMsgByMsgId(uint64_t msgid);

  std::string GetVoiceBuffByMsgId(uint64_t msgid);

  std::string GetPublicMsgCompressContentByMsgId(uint64_t msgid);

  std::string GetChatMsgStrContentByMsgId(uint64_t msgid);

  int64_t GetDbHandleByDbName(wchar_t *dbname);

 private:
  void AddDatebaseInfo(uint64_t storage);
  int ExecSelect(uint64_t db, const std::string &sql,
                 std::vector<std::vector<wechat::SqlResult>> &data);
  int ExecuteSQL(uint64_t db, const char *sql,
                 sqlite3::sqlite3_callback callback, void *data);

 private:
  std::unordered_map<std::wstring, DatabaseInfo> dbmap_;
  std::vector<DatabaseInfo> dbs_;
  sqlite3::SqliteFunction func_;
  uint64_t base_addr_;
  mutable std::recursive_mutex m;
};

}  // namespace wechat
#endif
