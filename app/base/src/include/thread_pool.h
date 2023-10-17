#ifndef BASE_THREAD_POOL_H_
#define BASE_THREAD_POOL_H_
#include "singleton.h"
#include <windows.h>

namespace base {

class ThreadPool :public Singleton<ThreadPool>{
 public:
  ~ThreadPool();

  bool Create(unsigned long min = 1, unsigned long max = 4);

  bool AddWork(PTP_WORK_CALLBACK callback,PVOID opt);

 private:
  PTP_POOL pool_;
  PTP_CLEANUP_GROUP cleanup_group_;
  TP_CALLBACK_ENVIRON env_;
};

}  // namespace wxhelper

#endif