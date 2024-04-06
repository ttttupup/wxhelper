#ifndef WXHELPER_WECHAT_DB_H_
#define WXHELPER_WECHAT_DB_H_
#include <map>
#include <string>
#include <vector>

#include "singleton.h"
#include "sqlite_function.h"
#include "wechat_interface.h"

namespace wechat {

class WeChatDb : public base::Singleton<WeChatDb> {
 public:
  void init(uint64_t base);
  int ExecuteSQL(uint64_t db, const char *sql,
                 sqlite3::sqlite3_callback callback, void *data);
 

 private:
  std::map<std::wstring,DatabaseInfo> dbmap_;
  std::vector<DatabaseInfo> dbs_;
  sqlite3::SqliteFunction func_;
  uint64_t base_addr_;
};

}  // namespace wechat
#endif
