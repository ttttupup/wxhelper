#ifndef WXHELEPER_HIDE_MODULE_H_
#define WXHELEPER_HIDE_MODULE_H_
#include <Windows.h>
namespace wxhelper {
typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
  ULONG Length;
  BOOLEAN Initialized;
  PVOID SsHandle;
  LIST_ENTRY InLoadOrderModuleList;
  LIST_ENTRY InMemoryOrderModuleList;
  LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
  LIST_ENTRY InLoadOrderModuleList;
  LIST_ENTRY InMemoryOrderModuleList;
  LIST_ENTRY InInitializationOrderModuleList;
  void* BaseAddress;
  void* EntryPoint;
  ULONG SizeOfImage;
  UNICODE_STRING FullDllName;
  UNICODE_STRING BaseDllName;
  ULONG Flags;
  SHORT LoadCount;
  SHORT TlsIndex;
  HANDLE SectionHandle;
  ULONG CheckSum;
  ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;


class HideModule {
 private:
  /* data */
 public:
  static void Hide(const char* module_name);
  static void Hide(HMODULE module);
};


}  // namespace wxhelper
#endif