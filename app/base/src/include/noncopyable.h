#ifndef BASE_NONCOPYABLE_H_
#define BASE_NONCOPYABLE_H_
class NonCopyable {
 protected:
  NonCopyable() {}
  ~NonCopyable() {}
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif