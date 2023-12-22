#ifndef BASE_MEMORY_H_
#define BASE_MEMORY_H_
#include <windows.h>
#include <vector>
namespace base {
namespace memory {
    std::vector<INT64> ScanAndMatchValue(INT64 value, INT64 start,int align);
    int sunday(const byte* total, int tlen, const byte* part, int plen);
}
}  // namespace base
#endif