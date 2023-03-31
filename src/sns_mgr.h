#ifndef WXHELPER_SNS_MGR_H_
#define WXHELPER_SNS_MGR_H_
#include "Windows.h"
#include "base_mgr.h"
namespace wxhelper{
    class SNSMgr:public BaseMgr{
        public:
        explicit  SNSMgr(DWORD base);
        ~SNSMgr();
        int GetFirstPage();
        int GetNextPage(ULONG64 sns_id);
    };
}
#endif