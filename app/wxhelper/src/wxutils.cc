#include "wxutils.h"

namespace wxhelper {
namespace wxutils {
UINT64 GetWeChatWinBase() { return (UINT64)GetModuleHandleA("WeChatWin.dll"); }
}  // namespace wxutils
}  // namespace wxhelper
