#ifndef WXHELPER_OFFSET_H_
#define WXHELPER_OFFSET_H_
#include <cstdint>
namespace wechat {
#define V_3_9_8_25 39825
#define V_3_9_9_43 39943
#define V_3_9_10_19 391019
#define V_3_9_11_19 391119
#define V_3_9_11_25 391125
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
#elif WECHAT_VERSION == V_3_9_9_43
const uint64_t kGetAccountServiceMgr = 0xa7df30;
const uint64_t kSyncMsg = 0xc39680;
const uint64_t kSyncMsgNext = 0xc39680;
const uint64_t kGetCurrentDataPath = 0x11664e0;
const uint64_t kGetAppDataSavePath = 0x14f29c0;
const uint64_t kGetSendMessageMgr = 0xa7c730;
const uint64_t kSendTextMsg = 0x11de090;
const uint64_t kFreeChatMsg = 0xa7dfb0;

const uint64_t kDoAddMsg = 0x1225a60;
const uint64_t kSendImageMsg = 0x11d3a70;
const uint64_t kChatMsgInstanceCounter = 0xa86820;
const uint64_t kSendFileMsg = 0xfeb750;
const uint64_t kGetAppMsgMgr = 0xa816d0;
const uint64_t kGetContactMgr = 0xa69fd0;
const uint64_t kGetContactList = 0x10b8420;

const uint64_t k_sqlite3_exec = 0x288ea10;
const uint64_t k_sqlite3_prepare = 0x2896590;
const uint64_t k_sqlite3_open = 0x28cdd90;
const uint64_t k_sqlite3_step = 0x2852a20;
const uint64_t k_sqlite3_column_count = 0x2853240;
const uint64_t k_sqlite3_column_name = 0x2853c40;
const uint64_t k_sqlite3_column_type = 0x2853a90;
const uint64_t k_sqlite3_column_blob = 0x2853270;
const uint64_t k_sqlite3_column_bytes = 0x2853360;
const uint64_t k_sqlite3_finalize = 0x2851ad0;

const uint64_t kGPInstance = 0x4076558;
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
const uint64_t kMultiDBMgr = 0x40ecf98;
const uint64_t kPublicMsgMgr = 0x40ea558;
const uint64_t kFavoriteStorageMgr = 0x40edb28;
const uint64_t kHardLinkMgr = 0x40ecec0;

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
const uint64_t kForwardMsg = 0x11dd780;

const uint64_t kOnSnsTimeLineSceneFinish = 0x1a73150;
const uint64_t kSNSGetFirstPage = 0x1a51dd0;
const uint64_t kSNSGetNextPageScene = 0x1a77240;
const uint64_t kSNSDataMgr = 0xeebda0;
const uint64_t kSNSTimeLineMgr = 0x19e83a0;
const uint64_t kGetMgrByPrefixLocalId = 0x105a010;
const uint64_t kAddFavFromMsg = 0x1601520;
const uint64_t kGetChatMgr = 0xaafd90;
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

const uint64_t kGetLockWechatMgr = 0xbadb10;
const uint64_t kRequestLockWechat = 0xb63770;
const uint64_t kRequestUnLockWechat = 0xb63a10;

const uint64_t kOnLoginBtnClick = 0xf4d0f0;

const uint64_t kGetQRCodeLoginMgr = 0xf3fa20;

const uint64_t kUpdateMsg = 0xf15c40;
const uint64_t kGetVoiceMgr = 0xbf78f0;
const uint64_t kChatMsg2NetSceneSendMsg = 0x96e8d0;
const uint64_t kTranslateVoice = 0x11217e0;
const uint64_t kNewWebViewPageConfig = 0x9512f0;
const uint64_t kFreeWebViewPageConfig = 0x951520;
const uint64_t kGetWebViewMgr = 0x9412d0;
const uint64_t kShowWebView = 0x1d236b0;
const uint64_t kSetUrl = 0x13dd410;
#elif WECHAT_VERSION == V_3_9_10_19
const uint64_t kGetAccountServiceMgr = 0x1c1fe70;
const uint64_t kSyncMsg = 0xc39680;
const uint64_t kSyncMsgNext = 0xc39680;
const uint64_t kGetCurrentDataPath = 0x2315ea0;
const uint64_t kGetAppDataSavePath = 0x26a7df0;
const uint64_t kGetSendMessageMgr = 0x1c1e670;
const uint64_t kSendTextMsg = 0x238ec70;
const uint64_t kFreeChatMsg = 0x1c1fef0;

const uint64_t kDoAddMsg = 0x23d6f50;
const uint64_t kSendImageMsg = 0x2384400;
const uint64_t kChatMsgInstanceCounter = 0x1c287e0;
const uint64_t kSendFileMsg = 0x2197a40;
const uint64_t kGetAppMsgMgr = 0x1c23610;
const uint64_t kGetContactMgr = 0x1c0bdc0;
const uint64_t kGetContactList = 0x22665a0;

const uint64_t k_sqlite3_exec = 0x3afba50;
const uint64_t k_sqlite3_prepare = 0x3b036d0;
const uint64_t k_sqlite3_open = 0x3b3aec0;
const uint64_t k_sqlite3_step = 0x3abfa50;
const uint64_t k_sqlite3_column_count = 0x3ac0270;
const uint64_t k_sqlite3_column_name = 0x3ac0c70;
const uint64_t k_sqlite3_column_type = 0x3ac0ac0;
const uint64_t k_sqlite3_column_blob = 0x3ac02a0;
const uint64_t k_sqlite3_column_bytes = 0x3ac0390;
const uint64_t k_sqlite3_finalize = 0x3abeb00;

const uint64_t kGPInstance = 0x5a415a0;
const uint64_t kMultiDBMgr = 0x5abc5c8;
const uint64_t kPublicMsgMgr = 0x5ab9b68;
const uint64_t kFavoriteStorageMgr = 0x5abd178;
const uint64_t kHardLinkMgr = 0x5abc4e8;

const uint64_t kChatRoomMgr = 0x1c4e1e0;
const uint64_t kGetChatRoomDetailInfo = 0x222cf00;
const uint64_t kNewChatRoomInfo = 0x25d0250;
const uint64_t kFreeChatRoomInfo = 0x25d0430;
const uint64_t kDoAddMemberToChatRoom = 0x221c900;
const uint64_t kDoModChatRoomMemberNickName = 0xe6db00;
const uint64_t kDelMemberFromChatRoom = 0x221cf40;
const uint64_t kGetMemberFromChatRoom = 0x222e750;
const uint64_t kNewChatRoom = 0x25cd670;
const uint64_t kFreeChatRoom = 0x25cd870;

const uint64_t kTopMsg = 0xa5e4f0;
const uint64_t kRemoveTopMsg = 0xe787b0;
const uint64_t kInviteMember = 0xe63650;
const uint64_t kHookLog = 0x1304e60;

const uint64_t kCreateChatRoom = 0xe63340;
const uint64_t kQuitChatRoom = 0xe6e3b0;
const uint64_t kForwardMsg = 0x11dd780;

const uint64_t kOnSnsTimeLineSceneFinish = 0x1a73150;
const uint64_t kSNSGetFirstPage = 0x1a51dd0;
const uint64_t kSNSGetNextPageScene = 0x1a77240;
const uint64_t kSNSDataMgr = 0xeebda0;
const uint64_t kSNSTimeLineMgr = 0x19e83a0;
const uint64_t kGetMgrByPrefixLocalId = 0x105a010;
const uint64_t kAddFavFromMsg = 0x1601520;
const uint64_t kGetChatMgr = 0xaafd90;
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

const uint64_t kGetLockWechatMgr = 0xbadb10;
const uint64_t kRequestLockWechat = 0xb63770;
const uint64_t kRequestUnLockWechat = 0xb63a10;

const uint64_t kOnLoginBtnClick = 0xf4d0f0;

const uint64_t kGetQRCodeLoginMgr = 0xf3fa20;

const uint64_t kUpdateMsg = 0xf15c40;
const uint64_t kGetVoiceMgr = 0xbf78f0;
const uint64_t kChatMsg2NetSceneSendMsg = 0x96e8d0;
const uint64_t kTranslateVoice = 0x11217e0;
const uint64_t kNewWebViewPageConfig = 0x9512f0;
const uint64_t kFreeWebViewPageConfig = 0x951520;
const uint64_t kGetWebViewMgr = 0x9412d0;
const uint64_t kShowWebView = 0x1d236b0;
const uint64_t kSetUrl = 0x13dd410;
#elif WECHAT_VERSION == V_3_9_11_19
const uint64_t kGetAccountServiceMgr = 0x1b50d00;
const uint64_t kSyncMsg = 0xc39680;
const uint64_t kSyncMsgNext = 0xc39680;
const uint64_t kGetCurrentDataPath = 0x2248ce0;
const uint64_t kGetAppDataSavePath = 0x25dc0e0;
const uint64_t kGetSendMessageMgr = 0x1b4f500;
const uint64_t kSendTextMsg = 0x22c2010;
const uint64_t kFreeChatMsg = 0x1b50d80;

const uint64_t kDoAddMsg = 0x230a3e0;
const uint64_t kSendImageMsg = 0x22b77a0;
const uint64_t kChatMsgInstanceCounter = 0x1b59670;
const uint64_t kSendFileMsg = 0x20cb6f0;
const uint64_t kGetAppMsgMgr = 0x1b544a0;
const uint64_t kGetContactMgr = 0x1b3ccd0;
const uint64_t kGetContactList = 0x219a1c0;

const uint64_t k_sqlite3_exec = 0x3a59b30;
const uint64_t k_sqlite3_prepare = 0x3a617b0;
const uint64_t k_sqlite3_open = 0x3a98fa0;
const uint64_t k_sqlite3_step = 0x3a1db30;
const uint64_t k_sqlite3_column_count = 0x3a1e350;
const uint64_t k_sqlite3_column_name = 0x3a1ed50;
const uint64_t k_sqlite3_column_type = 0x3a1eba0;
const uint64_t k_sqlite3_column_blob = 0x3a1e380;
const uint64_t k_sqlite3_column_bytes = 0x3a1e470;
const uint64_t k_sqlite3_finalize = 0x3a1cbe0;

const uint64_t kGPInstance = 0x58dd300;
const uint64_t kMultiDBMgr = 0x593abf8;
const uint64_t kPublicMsgMgr = 0x5938198;
const uint64_t kFavoriteStorageMgr = 0x593b790;
const uint64_t kHardLinkMgr = 0x593ab28;

const uint64_t kChatRoomMgr = 0x1b7f100;
const uint64_t kGetChatRoomDetailInfo = 0x2160bb0;
const uint64_t kNewChatRoomInfo = 0x2505120;
const uint64_t kFreeChatRoomInfo = 0x2505300;
const uint64_t kDoAddMemberToChatRoom = 0x21505b0;
const uint64_t kDoModChatRoomMemberNickName = 0x215a360;
const uint64_t kDelMemberFromChatRoom = 0x2150bf0;
const uint64_t kGetMemberFromChatRoom = 0x2162400;
const uint64_t kNewChatRoom = 0x2502540;
const uint64_t kFreeChatRoom = 0x2502740;

const uint64_t kTopMsg = 0x1d7d160;
const uint64_t kRemoveTopMsg = 0x2166090;
const uint64_t kInviteMember = 0x214ff90;
const uint64_t kHookLog = 0x1304e60;

const uint64_t kCreateChatRoom = 0x214fc60;
const uint64_t kQuitChatRoom = 0x215ac00;
const uint64_t kForwardMsg = 0x22c1590;

const uint64_t kOnSnsTimeLineSceneFinish = 0x1a73150;
const uint64_t kSNSGetFirstPage = 0x2e1bec0;
const uint64_t kSNSGetNextPageScene = 0x2e41a70;
const uint64_t kSNSDataMgr = 0x21dd6b0;
const uint64_t kSNSTimeLineMgr = 0x2dadf20;
const uint64_t kGetMgrByPrefixLocalId = 0x213afb0;
const uint64_t kAddFavFromMsg = 0x2930e40;
const uint64_t kGetChatMgr = 0x1b82bf0;
const uint64_t kGetFavoriteMgr = 0x1b57ba0;
const uint64_t kAddFavFromImage = 0x293d0f0;
const uint64_t kGetContact = 0x21945d0;
const uint64_t kNewContact = 0x2519300;
const uint64_t kFreeContact = 0x25199b0;
const uint64_t kNewMMReaderItem = 0x8c79a0;
const uint64_t kFreeMMReaderItem = 0x8c6da0;
const uint64_t kForwordPublicMsg = 0xddc6c0;
const uint64_t kParseAppMsgXml = 0x11b0a70;
const uint64_t kNewAppMsgInfo = 0x91a550;
const uint64_t kFreeAppMsgInfo = 0x8fd1a0;
const uint64_t kGetPreDownLoadMgr = 0x1c0a3a0;
const uint64_t kPushAttachTask = 0x1cda920;
const uint64_t kGetCustomSmileyMgr = 0x1ca0320;
const uint64_t kSendCustomEmotion = 0x21b04c0;
const uint64_t kNewJsApiShareAppMessage = 0x26cda20;
const uint64_t kInitJsConfig = 0x137bc00;
const uint64_t kSendApplet = 0x13c0920;
const uint64_t kSendAppletSecond = 0x13c1150;
const uint64_t kGetAppInfoByWaid = 0x13c5790;
const uint64_t kCopyShareAppMessageRequest = 0x13c0670;
const uint64_t kNewWAUpdatableMsgInfo = 0x919ca0;
const uint64_t kFreeWAUpdatableMsgInfo = 0x8fc230;
const uint64_t kSendPatMsg = 0x2ca9790;
const uint64_t kGetOCRManager = 0x999780;
const uint64_t kDoOCRTask = 0x2c53910;

const uint64_t kGetLockWechatMgr = 0x1c85010;
const uint64_t kRequestLockWechat = 0x1c397d0;
const uint64_t kRequestUnLockWechat = 0x1c39a70;

const uint64_t kOnLoginBtnClick = 0x202bc30;

const uint64_t kGetQRCodeLoginMgr = 0x201e350;

const uint64_t kUpdateMsg = 0x21421a0;
const uint64_t kGetVoiceMgr = 0x1e13320;
const uint64_t kChatMsg2NetSceneSendMsg = 0x1b70fd0;
const uint64_t kTranslateVoice = 0x2353d50;
const uint64_t kNewWebViewPageConfig = 0x1b53ae0;
const uint64_t kFreeWebViewPageConfig = 0x1b53d10;
const uint64_t kGetWebViewMgr = 0x1b43950;
const uint64_t kShowWebView = 0x302ed30;
const uint64_t kSetUrl = 0x13dd410;

const uint64_t kNewPayInfo = 0x1bcd930;
const uint64_t kFreePayInfo = 0x1b92450;
const uint64_t kTransferConfirm = 0x2f8c750;
const uint64_t kTransferRefuse = 0x2f8d340;

const uint64_t kAddFriend = 0x1f3a350;
const uint64_t kVerifyApply = 0x1f3a940;
const uint64_t kDoDelContact = 0x2310120;

const uint64_t kGetSearchContactMgr = 0x1f9a730;
const uint64_t kStartSearch = 0x22a4200;
#elif WECHAT_VERSION == V_3_9_11_25
const uint64_t kGetAccountServiceMgr = 0x1b50d00;
const uint64_t kSyncMsg = 0xc39680;
const uint64_t kSyncMsgNext = 0xc39680;
const uint64_t kGetCurrentDataPath = 0x2248ce0;
const uint64_t kGetAppDataSavePath = 0x25dc0e0;
const uint64_t kGetSendMessageMgr = 0x1b4f500;
const uint64_t kSendTextMsg = 0x22c2010;
const uint64_t kFreeChatMsg = 0x1b50d80;

const uint64_t kDoAddMsg = 0x230a3e0;
const uint64_t kSendImageMsg = 0x22b77a0;
const uint64_t kChatMsgInstanceCounter = 0x1b59670;
const uint64_t kSendFileMsg = 0x20cb6f0;
const uint64_t kGetAppMsgMgr = 0x1b544a0;
const uint64_t kGetContactMgr = 0x1b3ccd0;
const uint64_t kGetContactList = 0x219a1c0;

const uint64_t k_sqlite3_exec = 0x3a59b30;
const uint64_t k_sqlite3_prepare = 0x3a617b0;
const uint64_t k_sqlite3_open = 0x3a98fa0;
const uint64_t k_sqlite3_step = 0x3a1db30;
const uint64_t k_sqlite3_column_count = 0x3a1e350;
const uint64_t k_sqlite3_column_name = 0x3a1ed50;
const uint64_t k_sqlite3_column_type = 0x3a1eba0;
const uint64_t k_sqlite3_column_blob = 0x3a1e380;
const uint64_t k_sqlite3_column_bytes = 0x3a1e470;
const uint64_t k_sqlite3_finalize = 0x3a1cbe0;

const uint64_t kGPInstance = 0x58dd300;
const uint64_t kMultiDBMgr = 0x593abf8;
const uint64_t kPublicMsgMgr = 0x5938198;
const uint64_t kFavoriteStorageMgr = 0x593b790;
const uint64_t kHardLinkMgr = 0x593ab28;

const uint64_t kChatRoomMgr = 0x1b7f100;
const uint64_t kGetChatRoomDetailInfo = 0x2160bb0;
const uint64_t kNewChatRoomInfo = 0x2505120;
const uint64_t kFreeChatRoomInfo = 0x2505300;
const uint64_t kDoAddMemberToChatRoom = 0x21505b0;
const uint64_t kDoModChatRoomMemberNickName = 0x215a360;
const uint64_t kDelMemberFromChatRoom = 0x2150bf0;
const uint64_t kGetMemberFromChatRoom = 0x2162400;
const uint64_t kNewChatRoom = 0x2502540;
const uint64_t kFreeChatRoom = 0x2502740;

const uint64_t kTopMsg = 0x1d7d160;
const uint64_t kRemoveTopMsg = 0x2166090;
const uint64_t kInviteMember = 0x214ff90;
const uint64_t kHookLog = 0x1304e60;

const uint64_t kCreateChatRoom = 0x214fc60;
const uint64_t kQuitChatRoom = 0x215ac00;
const uint64_t kForwardMsg = 0x22c1590;

const uint64_t kOnSnsTimeLineSceneFinish = 0x1a73150;
const uint64_t kSNSGetFirstPage = 0x2e1bec0;
const uint64_t kSNSGetNextPageScene = 0x2e41a70;
const uint64_t kSNSDataMgr = 0x21dd6b0;
const uint64_t kSNSTimeLineMgr = 0x2dadf20;
const uint64_t kGetMgrByPrefixLocalId = 0x213afb0;
const uint64_t kAddFavFromMsg = 0x2930e40;
const uint64_t kGetChatMgr = 0x1b82bf0;
const uint64_t kGetFavoriteMgr = 0x1b57ba0;
const uint64_t kAddFavFromImage = 0x293d0f0;
const uint64_t kGetContact = 0x21945d0;
const uint64_t kNewContact = 0x2519300;
const uint64_t kFreeContact = 0x25199b0;
const uint64_t kNewMMReaderItem = 0x8c79a0;
const uint64_t kFreeMMReaderItem = 0x8c6da0;
const uint64_t kForwordPublicMsg = 0xddc6c0;
const uint64_t kParseAppMsgXml = 0x11b0a70;
const uint64_t kNewAppMsgInfo = 0x91a550;
const uint64_t kFreeAppMsgInfo = 0x8fd1a0;
const uint64_t kGetPreDownLoadMgr = 0x1c0a3a0;
const uint64_t kPushAttachTask = 0x1cda920;
const uint64_t kGetCustomSmileyMgr = 0x1ca0320;
const uint64_t kSendCustomEmotion = 0x21b04c0;
const uint64_t kNewJsApiShareAppMessage = 0x26cda20;
const uint64_t kInitJsConfig = 0x137bc00;
const uint64_t kSendApplet = 0x13c0920;
const uint64_t kSendAppletSecond = 0x13c1150;
const uint64_t kGetAppInfoByWaid = 0x13c5790;
const uint64_t kCopyShareAppMessageRequest = 0x13c0670;
const uint64_t kNewWAUpdatableMsgInfo = 0x919ca0;
const uint64_t kFreeWAUpdatableMsgInfo = 0x8fc230;
const uint64_t kSendPatMsg = 0x2ca9790;
const uint64_t kGetOCRManager = 0x999780;
const uint64_t kDoOCRTask = 0x2c53910;

const uint64_t kGetLockWechatMgr = 0x1c85010;
const uint64_t kRequestLockWechat = 0x1c397d0;
const uint64_t kRequestUnLockWechat = 0x1c39a70;

const uint64_t kOnLoginBtnClick = 0x202bc30;

const uint64_t kGetQRCodeLoginMgr = 0x201e350;

const uint64_t kUpdateMsg = 0x21421a0;
const uint64_t kGetVoiceMgr = 0x1e13320;
const uint64_t kChatMsg2NetSceneSendMsg = 0x1b70fd0;
const uint64_t kTranslateVoice = 0x2353d50;
const uint64_t kNewWebViewPageConfig = 0x1b53ae0;
const uint64_t kFreeWebViewPageConfig = 0x1b53d10;
const uint64_t kGetWebViewMgr = 0x1b43950;
const uint64_t kShowWebView = 0x302ed30;
const uint64_t kSetUrl = 0x13dd410;

const uint64_t kNewPayInfo = 0x1bcd930;
const uint64_t kFreePayInfo = 0x1b92450;
const uint64_t kTransferConfirm = 0x2f8c750;
const uint64_t kTransferRefuse = 0x2f8d340;

const uint64_t kAddFriend = 0x1f3a350;
const uint64_t kVerifyApply = 0x1f3a940;
const uint64_t kDoDelContact = 0x2310120;

const uint64_t kGetSearchContactMgr = 0x1f9a730;
const uint64_t kStartSearch = 0x22a4200;
#else
#ifdef WECHAT_VERSION
#error "Unsupported WeChat version."
#endif
#endif
}  // namespace offset
}  // namespace wechat

#endif