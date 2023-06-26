#ifndef WXHELPER_THREAD_POOL_H_
#define WXHELPER_THREAD_POOL_H_
#include "Windows.h"
#include "singleton.h"
namespace wxhelper {

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