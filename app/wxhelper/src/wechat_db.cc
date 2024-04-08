#include "wechat_db.h"

#include "base64.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"

namespace wechat {
void FreeResult(std::vector<std::vector<wechat::SqlResult>> &data) {
  if (data.empty()) {
    return;
  }
  for (auto &row : data) {
    for (auto &item : row) {
      if (item.column_name) {
        delete[] item.column_name;
        item.column_name = nullptr;
      }
      if (item.content) {
        delete[] item.content;
        item.content = nullptr;
      }
    }
    std::vector<wechat::SqlResult>().swap(row);
  }
  std::vector<std::vector<wechat::SqlResult>>().swap(data);
}

int GetDbInfoCallback(void *data, int argc, char **argv, char **name) {
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

void WeChatDb::Init() {
  base_addr_ = wxhelper::wxutils::GetWeChatWinBase();
  func_ = sqlite3::SqliteFunction(base_addr_);
  dbmap_ = {};
  dbs_ = {};
}

int WeChatDb::ExecuteSQL(uint64_t db, const char *sql,
                         sqlite3::sqlite3_callback callback, void *data) {
  return func_.sqlite3_exec(db, sql, callback, data, 0);
}

std::vector<void *> WeChatDb::GetWeChatDbHandles() {
  std::lock_guard<std::mutex> lock(m);
  dbs_.clear();
  dbmap_.clear();
  uint64_t p_contact_addr = *(uint64_t *)(base_addr_ + offset::kGPInstance);
  std::vector<uint64_t> addr_vector{
      0x180,  0x278,  0x3a0,  0x4c8,  0x5a0,  0x6e0,  0x838,  0x958,  0xa40,
      0xb28,  0xd00,  0xe10,  0xef8,  0xfd0,  0x10e8, 0x1200, 0x13d0, 0x15a0,
      0x1678, 0x17a8, 0x18c0, 0x1998, 0x1a70, 0x1b48, 0x1c48, 0x1d60, 0x1e38};

  for (uint64_t elem : addr_vector) {
    uint64_t db_addr = *(uint64_t *)(p_contact_addr + elem + 0x50);
    if (db_addr == 0) {
      continue;
    }
    std::wstring name =
        std::wstring((wchar_t *)(*(uint64_t *)(p_contact_addr + elem + 0x78)),
                     *(uint32_t *)(p_contact_addr + elem + 0x80));
    auto it = dbmap_.find(name);
    if (it != dbmap_.end()) {
      continue;
    } else {
      DatabaseInfo db{0};
      db.db_name = (wchar_t *)(*(uint64_t *)(p_contact_addr + elem + 0x78));
      db.db_name_len = *(int32_t *)(p_contact_addr + elem + 0x80);
      db.handle = db_addr;
      ExecuteSQL(db_addr, "select * from sqlite_master where type=\"table\";",
                 GetDbInfoCallback, &db);
      dbs_.push_back(db);
      dbmap_[name] = db;
    }
  }

  // MsgX.db
  uint64_t multi_db_mgr_addr = base_addr_ + offset::kMultiDBMgr;
  uint64_t wrap_ptr = *(uint64_t *)(multi_db_mgr_addr);
  uint64_t current_db_num = *(uint64_t *)(wrap_ptr + 0x68);
  uint64_t begin_ptr = *(uint64_t *)(wrap_ptr + 0x50);

  for (unsigned int i = 0; i < current_db_num; ++i) {
    uint64_t next_addr = begin_ptr + i * 0x8;
    uint64_t db_addr = *(uint64_t *)next_addr;
    if (db_addr) {
      uint64_t msg0_db_addr = *(uint64_t *)(db_addr + 0x78);
      DatabaseInfo msg0_db{0};
      msg0_db.db_name = (wchar_t *)(*(uint64_t *)(db_addr));
      msg0_db.db_name_len = *(int32_t *)(db_addr + 0x8);
      msg0_db.handle = msg0_db_addr;
      msg0_db.extrainfo = *(uint64_t *)(*(uint64_t *)(db_addr + 0x28) + 0x1E8);
      ExecuteSQL(msg0_db_addr,
                 "select * from sqlite_master where type=\"table\";",
                 GetDbInfoCallback, &msg0_db);
      dbs_.push_back(msg0_db);
      std::wstring msg_db_name =
          std::wstring(msg0_db.db_name, msg0_db.db_name_len);
      dbmap_[msg_db_name] = msg0_db;
      // BufInfoStorage
      uint64_t buf_info_addr = *(uint64_t *)(db_addr + 0x20);
      if (buf_info_addr) {
        AddDatebaseInfo(buf_info_addr);
      }
    }
  }

  // publicMsg.db
  uint64_t public_msg_mgr_addr = base_addr_ + offset::kPublicMsgMgr;
  uint64_t public_msg_mgr_ptr = *(uint64_t *)(public_msg_mgr_addr);
  for (unsigned int i = 1; i < 4; ++i) {
    uint64_t public_msg_ptr = *(uint64_t *)(public_msg_mgr_ptr + i * 0x8);
    if (public_msg_ptr) {
      AddDatebaseInfo(public_msg_ptr);
    }
  }
  // Favorite.db
  uint64_t favoite = base_addr_ + offset::kFavoriteStorageMgr;
  uint64_t item_ptr =
      *(uint64_t *)(*(uint64_t *)(*(uint64_t *)(favoite) + 0x10) + 0x8);
  if (item_ptr) {
    AddDatebaseInfo(item_ptr);
  }

  // HardLinkMgr
  uint64_t link = base_addr_ + offset::kHardLinkMgr;
  uint64_t link_ptr = *(uint64_t *)(*(uint64_t *)(link) + 0x158);
  for (unsigned int i = 0; i < 6; ++i) {
    uint64_t hard_link_ptr = *(uint64_t *)(link_ptr + i * 0x8);
    SPDLOG_INFO("hardlinkxxxdb :{}", hard_link_ptr);
    if (hard_link_ptr) {
      AddDatebaseInfo(hard_link_ptr);
    }
  }
  SPDLOG_INFO("db number:{}", dbs_.size());
  std::vector<void *> ret;
  for (unsigned int i = 0; i < dbs_.size(); ++i) {
    ret.push_back(&dbs_[i]);
  }
  return ret;
}

int WeChatDb::Select(uint64_t db_hanle, const std::string &sql,
                     std::vector<std::vector<std::string>> &query_result) {
  std::vector<std::vector<wechat::SqlResult>> data;
  int status = ExecSelect(db_hanle, sql, data);
  if (status != SQLITE_OK) {
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
        bool is_utf8 =
            base::utils::IsTextUtf8(it[i].content, it[i].content_len);
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

void WeChatDb::AddDatebaseInfo(uint64_t storage) {
  uint64_t storage_addr = *(uint64_t *)(storage + 0x50);
  std::wstring name = std::wstring((wchar_t *)(*(uint64_t *)(storage + 0x78)),
                                   *(int32_t *)(storage + 0x80));
  auto it = dbmap_.find(name);
  if (it == dbmap_.end()) {
    DatabaseInfo db{0};
    db.db_name = (wchar_t *)(*(uint64_t *)(storage + 0x78));
    db.db_name_len = *(int32_t *)(storage + 0x80);
    db.handle = storage_addr;
    ExecuteSQL(storage_addr,
               "select * from sqlite_master where type=\"table\";",
               GetDbInfoCallback, &db);
    dbs_.push_back(db);
    dbmap_[name] = db;
  }
}

int WeChatDb::ExecSelect(uint64_t db, const std::string &sql,
                         std::vector<std::vector<wechat::SqlResult>> &data) {
  uint64_t *stmt;
  int rc = func_.sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    return rc;
  }
  while (func_.sqlite3_step(stmt) == SQLITE_ROW) {
    int col_count = func_.sqlite3_column_count(stmt);
    std::vector<wechat::SqlResult> res;
    for (unsigned int i = 0; i < col_count; ++i) {
      wechat::SqlResult temp = {0};
      const char *col_name = func_.sqlite3_column_name(stmt, i);
      int ntype = func_.sqlite3_column_type(stmt, i);
      const void *p_blob_data = func_.sqlite3_column_blob(stmt, i);
      int nlength = func_.sqlite3_column_bytes(stmt, i);
      temp.column_name = new char[strlen(col_name) + 1];
      memcpy(temp.column_name, col_name, strlen(col_name) + 1);
      temp.column_name_len = strlen(col_name);
      temp.content_len = nlength;
      switch (ntype) {
        case SQLITE_BLOB: {
          temp.content = new char[nlength];
          memcpy(temp.content, p_blob_data, nlength);
          temp.is_blob = true;
          break;
        }
        default: {
          if (nlength != 0) {
            temp.content = new char[nlength + 1];
            memcpy(temp.content, p_blob_data, nlength + 1);
          } else {
            temp.content = new char[2];
            ZeroMemory(temp.content, 2);
          }
          temp.is_blob = false;
          break;
        }
      }
      res.push_back(temp);
    }
    data.push_back(res);
  }
  func_.sqlite3_finalize(stmt);
  return SQLITE_OK;
}
}  // namespace wechat