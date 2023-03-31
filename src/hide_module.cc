#include "pch.h"
#include "hide_module.h"


namespace wxhelper {

void HideModule::Hide(const char* module_name) {
  HMODULE hMod = ::GetModuleHandleA(module_name);
  PLIST_ENTRY Head, Cur;
  PPEB_LDR_DATA ldr;
  PLDR_MODULE ldm;

  __asm {
        mov eax, fs: [0x30]
        mov ecx, [eax + 0x0c]
        mov ldr, ecx
  }
  Head = &(ldr->InLoadOrderModuleList);
  Cur = Head->Flink;
  do {
    ldm = CONTAINING_RECORD(Cur, LDR_MODULE, InLoadOrderModuleList);
    if (hMod == ldm->BaseAddress) {
      ldm->InLoadOrderModuleList.Blink->Flink =
          ldm->InLoadOrderModuleList.Flink;
      ldm->InLoadOrderModuleList.Flink->Blink =
          ldm->InLoadOrderModuleList.Blink;
      ldm->InInitializationOrderModuleList.Blink->Flink =
          ldm->InInitializationOrderModuleList.Flink;
      ldm->InInitializationOrderModuleList.Flink->Blink =
          ldm->InInitializationOrderModuleList.Blink;
      ldm->InMemoryOrderModuleList.Blink->Flink =
          ldm->InMemoryOrderModuleList.Flink;
      ldm->InMemoryOrderModuleList.Flink->Blink =
          ldm->InMemoryOrderModuleList.Blink;
      break;
    }
    Cur = Cur->Flink;
  } while (Head != Cur);
}

void HideModule::Hide(HMODULE module) {
  void* peb_ptr = nullptr;
  _asm {
		PUSH EAX
		MOV  EAX, FS:[0x30]
		MOV  peb_ptr, EAX
		POP  EAX
  }
  void* ldr_ptr = *((void**)((unsigned char*)peb_ptr + 0xc));
  void* cur_ptr = *((void**)((unsigned char*)ldr_ptr + 0x0c));
  void* next_ptr = cur_ptr;
  do {
    void* next = *((void**)((unsigned char*)next_ptr));
    void* last = *((void**)((unsigned char*)next_ptr + 0x4));
    void* base_addr = *((void**)((unsigned char*)next_ptr + 0x18));
    if (base_addr == module) {
      *((void**)((unsigned char*)last)) = next;
      *((void**)((unsigned char*)next + 0x4)) = last;
      cur_ptr = next;
    }
    next_ptr = *((void**)next_ptr);
  } while (cur_ptr != next_ptr);
}
}  // namespace wxhelper
