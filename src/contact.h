#ifndef CONTACT_H_
#define CONTACT_H_
#include <vector>
#include "wechat_data.h"

int GetAllContact(vector<Contact> &vec);



int DelContact(wchar_t* wxid);
#endif