#ifndef WXHELPER_DB_CONTROLLER_H_
#define WXHELPER_DB_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class DbController : public http::HttpController<DbController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/getDBInfo", GetDBInfo);
  ADD_PATH("/api/execSql", ExecSql);
  PATHS_END

 public:
  static std::string GetDBInfo(std::string params);
  static std::string ExecSql(std::string params);
};
}  // namespace wxhelper

#endif