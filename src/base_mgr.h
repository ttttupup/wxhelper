#ifndef WXHELPER_BASE_MGR_H_
#define WXHELPER_BASE_MGR_H_
#include <Windows.h>
namespace wxhelper{
    class BaseMgr{
        public:
        explicit  BaseMgr(DWORD base);
        ~BaseMgr();
        protected:
         DWORD  base_addr_;
    };
}
#endif 