#ifndef WXHELPER_CONFIG_H_
#define WXHELPER_CONFIG_H_

namespace wxhelper {

class Config {
 public:
  Config(/* args */);
  ~Config();
  void Initialize();
  int  GetPort();

 private:
  int port_;
};
}  // namespace wxhelper
#endif