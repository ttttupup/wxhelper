#ifndef BASE_SINGLETON_H_
#define BASE_SINGLETON_H_
namespace base {
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
}  // namespace base
#endif