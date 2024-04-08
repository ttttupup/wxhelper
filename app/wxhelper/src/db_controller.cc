#include "db_controller.h"

#include "nlohmann/json.hpp"
#include "utils.h"
#include "wechat_db.h"
#include "json_utils.h"

namespace wxhelper {

std::string DbController::GetDBInfo(std::string params) {
  std::vector<void*> v = wechat::WeChatDb::GetInstance().GetWeChatDbHandles();
  nlohmann::json ret_data = {{"data", nlohmann::json::array()}};
  for (unsigned int i = 0; i < v.size(); ++i) {
    nlohmann::json db_info;
    db_info["tables"] = nlohmann::json::array();
    wechat::DatabaseInfo* db = reinterpret_cast<wechat::DatabaseInfo*>(v[i]);
    db_info["handle"] = db->handle;
    std::wstring dbname(db->db_name);
    db_info["databaseName"] = base::utils::WstringToUtf8(dbname);
    for (auto table : db->tables) {
      nlohmann::json table_info = {{"name", table.name},
                                   {"tableName", table.table_name},
                                   {"sql", table.sql},
                                   {"rootpage", table.rootpage}};
      db_info["tables"].push_back(table_info);
    }
    ret_data["data"].push_back(db_info);
  }
  ret_data["code"] = 1;
  ret_data["msg"] = "success";
  return ret_data.dump();
}

std::string DbController::ExecSql(std::string params) { 
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t db_handle = jsonutils::GetInt64Param(jp, "dbHandle");
  std::string sql = jsonutils::GetStringParam(jp, "sql");
  std::vector<std::vector<std::string>> items;
  int success = wechat::WeChatDb::GetInstance().Select(db_handle, sql.c_str(), items);
  nlohmann::json ret_data = {
      {"data", nlohmann::json::array()}, {"code", success}, {"msg", "success"}};
  if (success == 0) {
    ret_data["msg"] = "no data";
    return ret_data.dump();
  }
  for (auto it : items) {
    nlohmann::json temp_arr = nlohmann::json::array();
    for (size_t i = 0; i < it.size(); i++) {
      temp_arr.push_back(it[i]);
    }
    ret_data["data"].push_back(temp_arr);
  }
  return ret_data.dump();
  }

}  // namespace wxhelper
