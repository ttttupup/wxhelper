﻿#ifndef WXHELPER_ACCOUNT_MGR_H_
#define WXHELPER_ACCOUNT_MGR_H_
#include "wechat_function.h"
#include"base_mgr.h"
namespace wxhelper{
    class AccountMgr: public BaseMgr
    {
    public:
    explicit  AccountMgr(DWORD base);
     ~AccountMgr();

     std::string GetLoginUrl();

     int GetSelfInfo(SelfInfoInner& out);

     int CheckLogin();

     int Logout();

     std::string GetQRCodeUrl();

     int EnterWeChat();
    };
    
}
#endif