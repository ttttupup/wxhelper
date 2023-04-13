#ifndef WXHELPER_SINGLETON_H_
#define WXHELPER_SINGLETON_H_
template <typename T>
class Singleton {
 protected:
  Singleton() {}
  ~Singleton() {}

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

 public:
  static T& GetInstance() {
    static T instance{};
    return instance;
  }
};
#endif
