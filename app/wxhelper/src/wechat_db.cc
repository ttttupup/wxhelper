#include "wechat_db.h"
#include "wechat_interface.h"
namespace wechat {

void WeChatDb::init(uint64_t base) {
    base_addr_ = base;
  func_ = sqlite3::SqliteFunction(base);
  dbmap_ = {};
  dbs_ = {};
  
}


int WeChatDb::ExecuteSQL(uint64_t db, const char *sql,
                   sqlite3::sqlite3_callback callback, void *data) {
  return func_.sqlite3_exec(db, sql, callback, data, 0);
}









}  // namespace wechat