#ifndef HOOK_IMG_H_
#define HOOK_IMG_H_
#include  "windows.h"
using namespace std;

int HookImg(wstring save_path);
int UnHookImg();

int GetImgByName(wchar_t* file_path,wchar_t* save_dir);
#endif