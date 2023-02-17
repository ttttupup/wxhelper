#ifndef SEARCH_CONTACT_H_
#define SEARCH_CONTACT_H_
#include "wechat_data.h"
int SearchContactNetScene(wchar_t *keyword,UserInfo ** user);

int HookSearchContact();
int UnHookSearchContact();
#endif