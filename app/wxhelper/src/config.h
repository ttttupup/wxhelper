#ifndef WXHELPER_CONFIG_H_
#define WXHELPER_CONFIG_H_

namespace wxhelper {

class Config {
 public:
  Config();
  ~Config();
  void Initialize();
  int GetPort();
  int GetHideDll();

 private:
  int port_;
  int hidden_dll_;
};
}  // namespace wxhelper
#endif