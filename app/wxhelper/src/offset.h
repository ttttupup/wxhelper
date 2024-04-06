#ifndef WXHELPER_OFFSET_H_
#define WXHELPER_OFFSET_H_
#include <cstdint>
namespace wechat {
#define V_3_9_8_25 39825
#ifndef WECHAT_VERSION
#error " WECHAT_VERSION not defined ."
#endif
namespace offset {
#if WECHAT_VERSION == V_3_9_8_25
const uint64_t kGetAccountServiceMgr = 0x94e510;
const uint64_t kSyncMsg = 0xc39680;
const uint64_t kSyncMsgNext = 0xc39680;
const uint64_t kGetCurrentDataPath = 0x101a920;
const uint64_t kGetAppDataSavePath = 0x13a5b90;
const uint64_t kGetSendMessageMgr = 0x94cd10;
const uint64_t kSendTextMsg = 0x1091F70;
const uint64_t kFreeChatMsg = 0x94e590;

const uint64_t kDoAddMsg = 0x10d9450;
const uint64_t kSendImageMsg = 0x1087950;
const uint64_t kChatMsgInstanceCounter = 0x956e00;
const uint64_t kSendFileMsg = 0xea0850;
const uint64_t kGetAppMsgMgr = 0x951cb0;
const uint64_t kGetContactMgr = 0x93a570;
const uint64_t kGetContactList = 0xf6cb70;

const uint64_t k_sqlite3_exec = 0x26e4f20;
const uint64_t k_sqlite3_prepare = 0x26ecaa0;
const uint64_t k_sqlite3_open = 0x27242a0;
const uint64_t k_sqlite3_step = 0x26a8f30;
const uint64_t k_sqlite3_column_count = 0x26a9750;
const uint64_t k_sqlite3_column_name = 0x26aa150;
const uint64_t k_sqlite3_column_type = 0x26a9fa0;
const uint64_t k_sqlite3_column_blob = 0x26a9780;
const uint64_t k_sqlite3_column_bytes = 0x26a9870;
const uint64_t k_sqlite3_finalize = 0x26a7fe0;

const uint64_t kGPInstance = 0x3d8b4f8;
const uint64_t kMicroMsgDB = 0xb8;
const uint64_t kChatMsgDB = 0x2c8;
const uint64_t kMiscDB = 0x5f0;
const uint64_t kEmotionDB = 0x888;
const uint64_t kMediaDB = 0xF48;
const uint64_t kBizchatMsgDB = 0x1AC0;
const uint64_t kFunctionMsgDB = 0x1b98;
const uint64_t kDBName = 0x28;
const uint64_t kStorageStart = 0x0;
const uint64_t kStorageEnd = 0x0;
const uint64_t kMultiDBMgr = 0x3e00910;
const uint64_t kPublicMsgMgr = 0x3dfe098;
const uint64_t kFavoriteStorageMgr = 0x3e01478;

const uint64_t kChatRoomMgr = 0x8e9d30;
const uint64_t kGetChatRoomDetailInfo = 0xe73590;
const uint64_t kNewChatRoomInfo = 0x12006b0;
const uint64_t kFreeChatRoomInfo = 0x1200890;
const uint64_t kDoAddMemberToChatRoom = 0xe63c70;
const uint64_t kDoModChatRoomMemberNickName = 0xe6db00;
const uint64_t kDelMemberFromChatRoom = 0xe64290;
const uint64_t kGetMemberFromChatRoom = 0xe74de0;
const uint64_t kNewChatRoom = 0x11fde50;
const uint64_t kFreeChatRoom = 0x11fe030;

const uint64_t kTopMsg = 0xa5e4f0;
const uint64_t kRemoveTopMsg = 0xe787b0;
const uint64_t kInviteMember = 0xe63650;
const uint64_t kHookLog = 0x1304e60;

const uint64_t kCreateChatRoom = 0xe63340;
const uint64_t kQuitChatRoom = 0xe6e3b0;
const uint64_t kForwardMsg = 0x1091660;

const uint64_t kOnSnsTimeLineSceneFinish = 0x1a73150;
const uint64_t kSNSGetFirstPage = 0x1a51dd0;
const uint64_t kSNSGetNextPageScene = 0x1a77240;
const uint64_t kSNSDataMgr = 0xeebda0;
const uint64_t kSNSTimeLineMgr = 0x19e83a0;
const uint64_t kGetMgrByPrefixLocalId = 0xf0ea60;
const uint64_t kAddFavFromMsg = 0x1601520;
const uint64_t kGetChatMgr = 0x97e4d0;
const uint64_t kGetFavoriteMgr = 0x8c69b0;
const uint64_t kAddFavFromImage = 0x160b920;
const uint64_t kGetContact = 0xf67060;
const uint64_t kNewContact = 0x12e01f0;
const uint64_t kFreeContact = 0x12e08a0;
const uint64_t kNewMMReaderItem = 0x8c79a0;
const uint64_t kFreeMMReaderItem = 0x8c6da0;
const uint64_t kForwordPublicMsg = 0xddc6c0;
const uint64_t kParseAppMsgXml = 0x11b0a70;
const uint64_t kNewAppMsgInfo = 0x91a550;
const uint64_t kFreeAppMsgInfo = 0x8fd1a0;
const uint64_t kGetPreDownLoadMgr = 0x9996f0;
const uint64_t kPushAttachTask = 0x9c0080;
const uint64_t kGetCustomSmileyMgr = 0x915c00;
const uint64_t kSendCustomEmotion = 0xec0a40;
const uint64_t kNewJsApiShareAppMessage = 0x13be1a0;
const uint64_t kInitJsConfig = 0x137bc00;
const uint64_t kSendApplet = 0x13c0920;
const uint64_t kSendAppletSecond = 0x13c1150;
const uint64_t kGetAppInfoByWaid = 0x13c5790;
const uint64_t kCopyShareAppMessageRequest = 0x13c0670;
const uint64_t kNewWAUpdatableMsgInfo = 0x919ca0;
const uint64_t kFreeWAUpdatableMsgInfo = 0x8fc230;
const uint64_t kSendPatMsg = 0x195f340;
const uint64_t kGetOCRManager = 0x999780;
const uint64_t kDoOCRTask = 0x190b2a0;

const uint64_t kGetLockWechatMgr = 0xa727b0;
const uint64_t kRequestLockWechat = 0xa2cc70;
const uint64_t kRequestUnLockWechat = 0xa2cf10;

const uint64_t kOnLoginBtnClick = 0xe0cf70;

const uint64_t kGetQRCodeLoginMgr = 0xdff6d0;

const uint64_t kUpdateMsg = 0xf15c40;
const uint64_t kGetVoiceMgr = 0xbf78f0;
const uint64_t kChatMsg2NetSceneSendMsg = 0x96e8d0;
const uint64_t kTranslateVoice = 0x11217e0;
const uint64_t kNewWebViewPageConfig = 0x9512f0;
const uint64_t kFreeWebViewPageConfig = 0x951520;
const uint64_t kGetWebViewMgr = 0x9412d0;
const uint64_t kShowWebView = 0x1d236b0;
const uint64_t kSetUrl = 0x13dd410;
#elif WECHAT_VERSION == V_3_9_8_25

#else
#ifdef WECHAT_VERSION
#error "Unsupported WeChat version."
#endif
#endif
}  // namespace offset
}  // namespace wxhelper

#endif