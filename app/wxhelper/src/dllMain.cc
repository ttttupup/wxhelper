

#include "wxhelper.h"
BOOL APIENTRY DllMain(HMODULE module, DWORD ul_reason_for_call,
                      LPVOID reserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
      OutputDebugString("start\n"); 
      DisableThreadLibraryCalls(module);
      wxhelper::WxHelper::GetInstance().init(module);
      break;
    }
    case DLL_THREAD_ATTACH: {
      break;
    }
    case DLL_THREAD_DETACH: {
      break;
    }
    case DLL_PROCESS_DETACH: {
      OutputDebugString("end\n"); 
      wxhelper::WxHelper::GetInstance().finally();
      break;
    }
  }
  return TRUE;
}