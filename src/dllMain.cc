#include "pch.h"
#include "hide_module.h"
#include "global_context.h"


using namespace wxhelper;





BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
      DisableThreadLibraryCalls(hModule);
      GlobalContext::GetInstance().initialize(hModule);
      break;
    }
    case DLL_THREAD_ATTACH: {
      break;
    }
    case DLL_THREAD_DETACH: {
      break;
    }
    case DLL_PROCESS_DETACH: {
      GlobalContext::GetInstance().finally();
      break;
    }
  }
  return TRUE;
}
