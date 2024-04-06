#include "sync_msg_hook.h"

#include "base64.h"
#include "config.h"
#include "nlohmann/json.hpp"
#include "offset.h"
#include "spdlog/spdlog.h"
#include "thread_pool.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"

namespace wxhelper {

void SyncMsgHook::Init() {
  int64_t addr = wxutils::GetWeChatWinBase() + wechat::offset::kDoAddMsg;
  wechat::function::__DoAddMsg addMsg = (wechat::function::__DoAddMsg)addr;
  origin_ = addMsg;
  detour_ = &HandleSyncMsg;
}

void SyncMsgHook::HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3) {
  nlohmann::json msg;

  msg["pid"] = GetCurrentProcessId();
  msg["fromUser"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x18));
  msg["toUser"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x28));
  msg["content"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x30));
  msg["signature"] =
      wxhelper::wxutils::ReadWeChatStr(*(INT64 *)(param2 + 0x48));
  msg["msgId"] = *(INT64 *)(param2 + 0x60);
  msg["msgSequence"] = *(DWORD *)(param2 + 0x5C);
  msg["createTime"] = *(DWORD *)(param2 + 0x58);
  msg["displayFullContent"] =
      wxhelper::wxutils::ReadWeChatStr(*(INT64 *)(param2 + 0x50));
  DWORD type = *(DWORD *)(param2 + 0x24);
  msg["type"] = type;
  if (type == 3) {
    std::string img =
        wxhelper::wxutils::ReadSKBuiltinBuffer(*(INT64 *)(param2 + 0x40));
    SPDLOG_INFO("encode size:{}", img.size());
    msg["base64Img"] = base64_encode(img);
  }
  std::string jstr = msg.dump() + '\n';
  hook::InnerMessageStruct *inner_msg = new hook::InnerMessageStruct;
  inner_msg->buffer = new char[jstr.size() + 1];
  memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
  inner_msg->length = jstr.size();
  std::string mode = wxhelper::Config::GetInstance().GetRecvMessageMode();
  if (mode == "http") {
    bool add = base::ThreadPool::GetInstance().AddWork(
        hook::SendHttpMsgCallback, inner_msg);
    SPDLOG_INFO("add http msg work:{}", add);
  } else if (mode == "tcp") {
    bool add = base::ThreadPool::GetInstance().AddWork(hook::SendTcpMsgCallback,
                                                       inner_msg);
    SPDLOG_INFO("add tcp msg work:{}", add);
  }
  int64_t addr =
      wxhelper::wxutils::GetWeChatWinBase() + wechat::offset::kDoAddMsg;
  wechat::function::__DoAddMsg addMsg = (wechat::function::__DoAddMsg)addr;
  addMsg(param1, param2, param3);
}
}  // namespace wxhelper