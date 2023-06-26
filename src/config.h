#ifndef WXHELPER_CONFIG_H_
#define WXHELPER_CONFIG_H_

namespace wxhelper {

class Config {
 public:
  Config();
  ~Config();
  void Initialize();
  int GetPort();

 private:
  int port_;
};
}  // namespace wxhelper
#endif