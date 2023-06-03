#include "pch.h"
#include "send_message_mgr.h"
#include "wechat_function.h"
#include "db.h"
#include "contact_mgr.h"
#include "spdlog/spdlog.h"
#include "base64.h"
#include "lz4.h"
#include "tinyxml2.h"

namespace wxhelper {
SendMessageMgr::SendMessageMgr(DWORD base):BaseMgr(base) {}
SendMessageMgr::~SendMessageMgr() {}
int SendMessageMgr::SendText(wchar_t* wxid, wchar_t* msg) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString text_msg(msg);
  wchar_t** msg_pptr = &text_msg.ptr;
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base_addr_ + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2D8] = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr
      PUSH       0x0
      PUSH       0x0
      PUSH       0x0
      PUSH       0x1
      PUSH       0x0
      MOV        EAX,msg_pptr
      PUSH       EAX
      LEA        EDX,to_user
      LEA        ECX,chat_msg
      CALL       send_text_msg_addr 
      MOV        success,EAX
      ADD        ESP,0x18
      LEA        ECX,chat_msg        
      CALL       free_chat_msg_addr
      POPFD
      POPAD
  }
  return success;
}
int SendMessageMgr::SendAtText(wchar_t* chat_room_id, wchar_t** wxids, int len,
                               wchar_t* msg) {
  int success = -1;
  WeChatString  * at_users = new WeChatString[len+1];
  std::wstring at_msg = L"";
  int number =0;
  for (int i = 0; i < len; i++) {
    std::wstring nickname;
    if (!lstrcmpiW((wchar_t *)wxids[i], (wchar_t *)L"notify@all")) {
      nickname = L"所有人";
    } else {
      ContactMgr contact{base_addr_};
      nickname = contact.GetContactOrChatRoomNickname(wxids[i]);
    }
    if (nickname.length() == 0) {
      continue;
    }

    WeChatString temp = {0};
    temp.ptr = (wchar_t *)wxids[i];
    temp.length = wcslen((wchar_t *)wxids[i]);
    temp.max_length = wcslen((wchar_t *)wxids[i]) * 2;
    memcpy(&at_users[number], &temp, sizeof(WeChatString));
    at_msg = at_msg + L"@" + nickname + L" ";
    number++;
  }
  if (number < 1){
    return success;
  }
  std::wstring origin(msg);
  at_msg += origin;
  AtInner at_list = {0};
  at_list.start = (DWORD)at_users;
  at_list.finsh = (DWORD)&at_users[number];
  at_list.end = (DWORD)&at_users[number];
  WeChatString to_user(chat_room_id);
  WeChatString text_msg((wchar_t *)at_msg.c_str());
  wchar_t **msg_pptr = &text_msg.ptr;
  
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base_addr_ + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2D8] = {0};
  __asm{
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr
      PUSH       0x0
      PUSH       0x0
      PUSH       0x0
      PUSH       0x1
      LEA        EAX,at_list
      PUSH       EAX
      MOV        EAX,msg_pptr
      PUSH       EAX
      LEA        EDX,to_user
      LEA        ECX,chat_msg
      CALL       send_text_msg_addr 
      MOV        success,EAX
      ADD        ESP,0x18
      LEA        ECX,chat_msg        
      CALL       free_chat_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendText  code = {}",success);
  return success;
}
int SendMessageMgr::SendImage(wchar_t* wxid, wchar_t* image_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(image_path);
  char chat_msg[0x2D8] = {0};
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_image_msg_addr = base_addr_ + WX_SEND_IMAGE_OFFSET;
  DWORD free_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr                        
      SUB        ESP,0x14
      MOV        temp,EAX
      LEA        EAX,null_obj
      MOV        ECX,ESP
      LEA        EDI,path
      PUSH       EAX
      CALL       init_chat_msg_addr                                   
      MOV        ECX,dword ptr [temp]
      LEA        EAX,to_user
      PUSH       EDI
      PUSH       EAX
      LEA        EAX,chat_msg
      PUSH       EAX
      CALL       send_image_msg_addr
      MOV        success,EAX
      LEA        ECX,chat_msg
      CALL       free_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendImage  code = {}",success);
  return success;
}
int SendMessageMgr::SendFile(wchar_t* wxid, wchar_t* file_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(file_path);
  char chat_msg[0x2D8] = {0};
  DWORD app_msg_mgr_addr = base_addr_ + WX_APP_MSG_MGR_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_file_addr = base_addr_ + WX_SEND_FILE_OFFSET;
  DWORD free_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       app_msg_mgr_addr  
      SUB        ESP,0x14
      MOV        temp,EAX
      LEA        EAX,null_obj
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr 
      PUSH       0x0 
      SUB        ESP,0x14
      MOV        EDI,ESP
      MOV        dword ptr [EDI],0  
      MOV        dword ptr [EDI + 0x4],0  
      MOV        dword ptr [EDI + 0x8],0  
      MOV        dword ptr [EDI + 0xc],0  
      MOV        dword ptr [EDI + 0x10],0  
      SUB        ESP,0x14
      LEA        EAX,path
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr                                   
      SUB        ESP,0x14
      LEA        EAX,to_user
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr                                    
      MOV        ECX,dword ptr [temp]
      LEA        EAX,chat_msg
      PUSH       EAX
      CALL       send_file_addr
      MOV        AL,byte ptr [eax + 0x38]
      MOVZX      EAX,AL                         
      MOV        success,EAX
      LEA        ECX,chat_msg
      CALL       free_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendFile  code = {}",success);
  if (success == 0x31) {
    return 1;
  }
  return success;
}

int SendMessageMgr::ForwardMsg(wchar_t* wxid, unsigned long long msgid) {
  int success = 0;

  int db_index = 0;
  int localid = DB::GetInstance().GetLocalIdByMsgId(msgid, db_index);

  if (localid == 0) return 0;
  WeChatString to_user(wxid);
  DWORD forward_msg_addr = base_addr_ + WX_FORWARD_MSG_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  __asm {
    PUSHAD
    PUSHFD
    MOV        EDX, DWORD PTR [db_index]
    PUSH       EDX
    MOV        EAX, DWORD PTR [localid]
    PUSH       EAX
    SUB        ESP,0x14
    MOV        ECX,ESP
    LEA        ESI,to_user
    PUSH       ESI
    CALL       init_chat_msg_addr      
    XOR        ECX,ECX                          
    CALL       forward_msg_addr
    MOVZX      EAX,AL
    MOV        success,EAX
    ADD        ESP,0x1c
    POPFD
    POPAD
  }
  return success;
}

int SendMessageMgr::ForwardPublicMsg(wchar_t* wxid, wchar_t* title, wchar_t* url, wchar_t* thumburl,wchar_t* senderId,wchar_t* senderName,wchar_t* digest) {
  int success = -1;
  char buff[0x238] = {0};
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD app_msg_mgr_addr = base_addr_ + WX_APP_MSG_MGR_OFFSET;
  DWORD new_item_addr = base_addr_ + NEW_MM_READ_ITEM_OFFSET;
  DWORD free_item_2_addr = base_addr_ + FREE_MM_READ_ITEM_2_OFFSET;
  DWORD forward_public_msg_addr = base_addr_ + FORWARD_PUBLIC_MSG_OFFSET;
  __asm {
    PUSHAD
    PUSHFD
    LEA        ECX,buff
    CALL       new_item_addr
    POPFD
    POPAD
  }
  WeChatString to_user(wxid);
  WeChatString wtitle(title);
  WeChatString wurl(url);
  WeChatString wthumburl(thumburl);
  WeChatString wsender(senderId);
  WeChatString wname(senderName);
  WeChatString wdigest(digest);
  memcpy(&buff[0x4], &wtitle, sizeof(wtitle));
  memcpy(&buff[0x2c], &wurl, sizeof(wurl));
  memcpy(&buff[0x6c], &wthumburl, sizeof(wthumburl));
  memcpy(&buff[0x94], &wdigest, sizeof(wdigest));
  memcpy(&buff[0x1A0], &wsender, sizeof(wsender));
  memcpy(&buff[0x1B4], &wname, sizeof(wname));
  __asm {
    PUSHAD
    PUSHFD
    CALL       app_msg_mgr_addr
    LEA        ECX,buff
    PUSH       ECX
    SUB        ESP,0x14
    MOV        EDI,EAX
    MOV        ECX,ESP
    LEA        EBX,to_user
    PUSH       EBX
    CALL       init_chat_msg_addr                                  
    MOV        ECX,EDI
    CALL       forward_public_msg_addr
    MOV        success,EAX
    ADD        EBX,0x14
    LEA        ECX,buff
    PUSH       0x0
    CALL       free_item_2_addr
    POPFD
    POPAD
  }
  return success;
}

int SendMessageMgr::ForwardPublicMsgByMsgSvrId(wchar_t *wxid, unsigned long long msgid){

  std::string compress_content =DB::GetInstance().GetPublicMsgCompressContentByMsgId(msgid);
  if (compress_content.empty()) {
    SPDLOG_INFO("Get compressContent is null from PublicMsg.db");
    return -3;
  }

  std::string decode = base64_decode(compress_content);
  size_t len = decode.size();
  const char* src = decode.c_str() ;
  size_t dst_len = (len << 8) ;
  char* dst = new char[dst_len];

  int decompress_len = LZ4_decompress_safe_partial(src,dst,len,dst_len,dst_len);
  if (decompress_len < 0) {
    SPDLOG_INFO("decompress content size :{}",decompress_len);
    return -1;
  }
  tinyxml2::XMLDocument doc;
  if(doc.Parse(dst,decompress_len-1) != 0){
    SPDLOG_INFO("tinyxml2 parse error");
    return -2;
  }
  const char* title = doc.FirstChildElement( "msg" )->FirstChildElement( "appmsg" )->FirstChildElement("title")->GetText();
  const char* digest = doc.FirstChildElement( "msg" )->FirstChildElement( "appmsg" )->FirstChildElement("des")->GetText();

  const char* url = doc.FirstChildElement("msg")
                        ->FirstChildElement("appmsg")
                        ->FirstChildElement("mmreader")
                        ->FirstChildElement("category")
                        ->FirstChildElement("item")
                        ->FirstChildElement("url")
                        ->GetText();
  const char* thumburl = doc.FirstChildElement("msg")
                             ->FirstChildElement("appmsg")
                             ->FirstChildElement("thumburl")
                             ->GetText();
  const char* username = doc.FirstChildElement("msg")
                             ->FirstChildElement("appmsg")
                             ->FirstChildElement("mmreader")
                             ->FirstChildElement("publisher")
                             ->FirstChildElement("username")
                             ->GetText();

  const char* nickname = doc.FirstChildElement("msg")
                             ->FirstChildElement("appmsg")
                             ->FirstChildElement("mmreader")
                             ->FirstChildElement("publisher")
                             ->FirstChildElement("nickname")
                             ->GetText();

  std::string stitle(title);
  std::string sdigest(digest);
  std::string surl(url);
  std::string sthumburl(thumburl);
  std::string susername(username);
  std::string snickname(nickname);

  std::wstring wstitle =Utils::UTF8ToWstring(stitle);
  std::wstring wsdigest =Utils::UTF8ToWstring(sdigest);
  std::wstring wsurl =Utils::UTF8ToWstring(surl);
  std::wstring wsthumburl =Utils::UTF8ToWstring(sthumburl);
  std::wstring wsusername =Utils::UTF8ToWstring(susername);
  std::wstring wsnickname =Utils::UTF8ToWstring(snickname);

 int code =  ForwardPublicMsg(wxid, const_cast<wchar_t*>(wstitle.c_str()),
                   const_cast<wchar_t*>(wsurl.c_str()),
                   const_cast<wchar_t*>(wsthumburl.c_str()),
                   const_cast<wchar_t*>(wsusername.c_str()),
                   const_cast<wchar_t*>(wsnickname.c_str()),
                   const_cast<wchar_t*>(wsdigest.c_str())

  );

 return code;
}

int SendMessageMgr::SendAppletMsg(wchar_t* wxid, wchar_t* appletid){
  int success = -1;
  
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD new_share_app_msg_req_addr = base_addr_ + NEW_SHARE_APP_MSG_REQ_OFFSET;
  DWORD free_share_app_msg_req_addr = base_addr_ + FREE_SHARE_APP_MSG_REQ_OFFSET;
  DWORD send_app_msg_addr = base_addr_ + SEND_APP_MSG_OFFSET;
  DWORD new_wa_updatable_msg_info_addr = base_addr_ + NEW_WA_UPDATABLE_MSG_INFO_OFFSET;
  DWORD free_wa_addr = base_addr_ + FREE_WA_UPDATABLE_MSG_INFO_OFFSET;

  std::wstring applet_id(appletid);
  std::string s_applet_id = Utils::WCharToUTF8(appletid);
  const char * c_applet_id = s_applet_id.c_str();

  std::vector<WeChatString> receiver_list;
  WeChatString wwxid(wxid);
  receiver_list.push_back(wwxid);
  VectorInner* list = (VectorInner*)&receiver_list;
  DWORD receiver_list_ptr = (DWORD)&list->start;

  WeChatString wapplet(appletid);
  char req[0x268] = {0};
  char temp[0x4] = {0};
  char buff[0x88] = {0};
  DWORD flag = 0xF;
  DWORD zero = 0;
  WeChatStr clean;
  WeChatStr app_id(c_applet_id);

  __asm{
    PUSHAD
    PUSHFD
    LEA        ECX,req
    CALL       new_share_app_msg_req_addr
    POPFD
    POPAD
  }
  // 0x40  gh   
  // 0xE8  img
  memcpy(&req[0x4], &app_id, sizeof(app_id));

  __asm{
    PUSHAD
    PUSHFD    
    LEA        ECX,buff
    CALL       new_wa_updatable_msg_info_addr
    LEA        EAX,buff
    PUSH       EAX
    SUB        ESP,0x14
    MOV        ESI,ESP
    MOV        dword ptr [EBP - 0x280],ESP
    MOV        dword ptr [EBP - 0x27c],ESI
    MOV        dword ptr [ESI],0x0
    MOV        dword ptr [ESI + 0x4],0
    MOV        dword ptr [ESI + 0x8],0
    MOV        dword ptr [ESI + 0xc],0
    MOV        dword ptr [ESI + 0x10],0
                                 
    LEA        EAX,req
    PUSH       EAX
    MOV        EAX,dword ptr[receiver_list_ptr]
    PUSH       EAX
    SUB        ESP,0x14
    LEA        EAX,wapplet
    MOV        ECX,ESP
    PUSH       EAX
    CALL       init_chat_msg_addr                                  
    LEA        ECX,temp
    CALL       send_app_msg_addr                           
    MOV        success,EAX       
    LEA        ECX,buff
    CALL       free_wa_addr                            
    POPFD
    POPAD
  }

// no need for WeChat to release memory
memcpy(&req[0x4], &clean, sizeof(clean));
memcpy(&req[0xC8], &zero, sizeof(zero));
memcpy(&req[0xCC], &flag, sizeof(flag));
__asm {
    PUSHAD
    PUSHFD   
    LEA        ECX,req
    CALL       free_share_app_msg_req_addr
    POPFD
    POPAD
}
return success;
}
}  // namespace wxhelperg