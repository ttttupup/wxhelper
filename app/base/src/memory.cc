#include "include/memory.h"

namespace base {
namespace memory {
std::vector<INT64> ScanAndMatchValue(INT64 value, INT64 start, int align) {
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);
  std::vector<INT64> result;
  INT64 min_addr =
      reinterpret_cast<INT64>(sys_info.lpMinimumApplicationAddress);
  INT64 max_addr =
      reinterpret_cast<INT64>(sys_info.lpMaximumApplicationAddress);
  const INT64 page_size = sys_info.dwPageSize;
  min_addr = min_addr > start ? min_addr : start;

  auto current_addr = min_addr;
  MEMORY_BASIC_INFORMATION mem_info = {};
  HANDLE handle = GetCurrentProcess();
  while (current_addr < max_addr) {
    VirtualQueryEx(handle, reinterpret_cast<LPVOID>(current_addr), &mem_info,
                   sizeof(MEMORY_BASIC_INFORMATION));

    if ((INT64)mem_info.RegionSize <= 0) {
      break;
    }
    INT64 region_size = mem_info.RegionSize;
    if ((mem_info.State & MEM_COMMIT) == MEM_COMMIT &&
        (mem_info.Protect & PAGE_GUARD) != PAGE_GUARD &&
        (mem_info.Protect & PAGE_NOACCESS) != PAGE_NOACCESS) {
      for (INT64 i = 0; i < region_size; i += align) {
        if (value == *(INT64 *)current_addr) {
          result.push_back(current_addr);
        }
        current_addr += align;
      }
    } else {
      current_addr += region_size;
    }
  }
  return result;
}

int Sunday(const byte* total, int tlen, const byte* part, int plen) {
  byte move[128] = {0};
  for (int i = 0; i < plen; i++) {
    move[part[i]] = plen - i;
  }
  int s = 0;
  int j;
  while (s <= tlen - plen) {
    j = 0;
    while (total[s + j] == part[j]) {
      j++;
      if (j == plen) {
        return s;
      }
    }
    s += move[total[s + plen]];
  }
  return -1;
}

}  // namespace memory
}  // namespace base