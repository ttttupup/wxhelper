#ifndef WXHELPER_CHAT_CONTROLLER_H_
#define WXHELPER_CHAT_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class MiscController : public http::HttpController<MiscController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/checkLogin", CheckLogin);
  ADD_PATH("/api/userInfo", GetUserInfo);
  ADD_PATH("/api/getSNSFirstPage", GetSNSFirstPage);
  ADD_PATH("/api/getSNSNextPage", GetSNSNextPage);
  ADD_PATH("/api/addFavFromMsg", AddFavFromMsg);

  ADD_PATH("/api/addFavFromImage", AddFavFromImage);
  ADD_PATH("/api/decodeImage", DecodeImage);
  ADD_PATH("/api/getVoiceByMsgId", GetVoiceByMsgId);
  ADD_PATH("/api/ocr", DoOcrTask);

  ADD_PATH("/api/lockWeChat", LockWeChat);
  ADD_PATH("/api/unlockWeChat", UnlockWeChat);
  ADD_PATH("/api/clickEnterWeChat", ClickEnterWeChat);
  ADD_PATH("/api/getLoginUrl", GetLoginUrl);

  ADD_PATH("/api/translateVoice", TranslateVoice);
  ADD_PATH("/api/getTranslateVoiceText", GetTranslateVoiceText);
  ADD_PATH("/api/openUrlByWeChat", OpenUrlByWeChat);
  ADD_PATH("/api/confirmReceipt", ConfirmReceipt);
  ADD_PATH("/api/downloadAttach", DownloadAttach);

  PATHS_END

 public:
  /// @brief 检查是否登录
  /// @param params json
  /// @return json
  static std::string CheckLogin(std::string params);
  /// @brief 获取登录用户信息
  /// @param params json
  /// @return json
  static std::string GetUserInfo(std::string params);
  /// @brief 朋友圈首页
  /// @param params json
  /// @return json
  static std::string GetSNSFirstPage(std::string params);
  /// @brief 朋友圈下一页
  /// @param params json
  /// @return json
  static std::string GetSNSNextPage(std::string params);
  /// @brief 收藏消息
  /// @param params json
  /// @return json
  static std::string AddFavFromMsg(std::string params);
  /// @brief 收藏图片
  /// @param params json
  /// @return json
  static std::string AddFavFromImage(std::string params);
  /// @brief 解码图片
  /// @param params json
  /// @return json
  static std::string DecodeImage(std::string params);
  /// @brief 获取语音文件
  /// @param params json
  /// @return json
  static std::string GetVoiceByMsgId(std::string params);
  /// @brief 图片ocr
  /// @param params json
  /// @return json
  static std::string DoOcrTask(std::string params);
  /// @brief 锁定微信
  /// @param params json
  /// @return json
  static std::string LockWeChat(std::string params);
  /// @brief 解锁微信
  /// @param params json
  /// @return json
  static std::string UnlockWeChat(std::string params);
  /// @brief 进入微信
  /// @param params json
  /// @return json
  static std::string ClickEnterWeChat(std::string params);
  /// @brief 获取登录url
  /// @param params json
  /// @return json
  static std::string GetLoginUrl(std::string params);
  /// @brief 语音转文本
  /// @param params json
  /// @return json
  static std::string TranslateVoice(std::string params);
  /// @brief 获取语音转文本结果
  /// @param params json
  /// @return json
  static std::string GetTranslateVoiceText(std::string params);
  /// @brief 通过浏览器打开url
  /// @param params json
  /// @return json
  static std::string OpenUrlByWeChat(std::string params);
  /// @brief 确认收款
  /// @param params json
  /// @return json
  static std::string ConfirmReceipt(std::string params);
  /// @brief 下载附件
  /// @param params json
  /// @return json
  static std::string DownloadAttach(std::string params);
};
}  // namespace wxhelper

#endif