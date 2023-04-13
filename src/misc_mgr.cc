#include "misc_mgr.h"

#include "pch.h"
#include "wechat_function.h"
#include "base64.h"
#include "db.h"
#include "hooks.h"
#include "easylogging++.h"
#define BUFSIZE 1024

#define JPEG0 0xFF
#define JPEG1 0xD8
#define JPEG2 0xFF
#define PNG0 0x89
#define PNG1 0x50
#define PNG2 0x4E
#define BMP0 0x42
#define BMP1 0x4D
#define GIF0 0x47
#define GIF1 0x49
#define GIF2 0x46
using namespace std;
namespace wxhelper {
MiscMgr::MiscMgr(DWORD base) : BaseMgr::BaseMgr(base) {}
MiscMgr::~MiscMgr() {}
int MiscMgr::SendPatMsg(wchar_t *chat_room_id, wchar_t *wxid) {
  int success = -1;
  WeChatString chat_room(chat_room_id);
  WeChatString self_wxid(wxid);
  DWORD get_pat_mgr_addr = base_addr_ + WX_PAT_MGR_OFFSET;
  DWORD send_pat_msg_addr = base_addr_ + WX_SEND_PAT_MSG_OFFSET;
  DWORD ret_addr = base_addr_ + WX_RET_OFFSET;
  __asm {
    PUSHAD
    CALL       get_pat_mgr_addr
    PUSH       ret_addr
    PUSH       0x0
    PUSH       EAX
    LEA        ECX,chat_room
    LEA        EDX,self_wxid
    CALL       send_pat_msg_addr                           
    ADD        ESP,0xc
    MOVZX      EAX,AL
    MOV        success,EAX      
    POPAD
  }
  return success;
}

int MiscMgr::DoOCRTask(wchar_t *img_path, std::string &result) {
  int success = -1;
  WeChatString path(img_path);
  WeChatString null_obj = {0};
  WeChatString ocr_result = {0};
  DWORD ocr_manager_addr = base_addr_ + WX_OCR_MANAGER_OFFSET;
  DWORD do_ocr_task_addr = base_addr_ + WX_DO_OCR_TASK_OFFSET;
  DWORD init_addr = base_addr_ + WX_INIT_OBJ_OFFSET;
  DWORD flag = 0;
  __asm {
        PUSHAD
        PUSHFD
        LEA        ECX,ocr_result
        CALL       init_addr
        CALL       ocr_manager_addr                                  
        LEA        ECX,null_obj
        PUSH       ECX
        LEA        ECX,flag
        PUSH       ECX
        LEA        ECX,ocr_result
        PUSH       ECX
        PUSH       0x0
        LEA        ECX,path
        PUSH       ECX
        MOV        ECX,EAX
        CALL       do_ocr_task_addr    
        MOV        success,EAX  
        POPFD
        POPAD
  }

  if (success == 0) {
    DWORD addr = (DWORD)&ocr_result;
    DWORD ptr = *(DWORD *)addr;
    DWORD num = *(DWORD *)(addr + 0x4);
    if (num <= 0) {
      return success;
    }

    DWORD header = *(DWORD *)ptr;
    for (unsigned int i = 0; i < num - 1; i++) {
      DWORD content = *(DWORD *)header;
      result += Utils::WstringToUTF8(READ_WSTRING(content, 0x14));

      header = content;
    }
  }
  return success;
}

int MiscMgr::DoConfirmReceipt(wchar_t *wxid, wchar_t *transcationid,
                     wchar_t *transferid) {
  int success = -1;
  WeChatString recv_id(wxid);
  WeChatString transcation_id(transcationid);
  WeChatString transfer_id(transferid);
  char pay_info[0x134] = {0};
  DWORD new_pay_info_addr = base_addr_ + WX_NEW_WCPAYINFO_OFFSET;
  DWORD free_pay_info_addr = base_addr_ + WX_FREE_WCPAYINFO_OFFSET;
  DWORD do_confirm_addr = base_addr_ + WX_CONFIRM_RECEIPT_OFFSET;
  __asm {
    PUSHAD
    LEA        ECX,pay_info
    CALL       new_pay_info_addr
    MOV        dword ptr [pay_info + 0x4], 0x1
    MOV        dword ptr [pay_info + 0x4C], 0x1        
    POPAD
  }
  memcpy(&pay_info[0x1c], &transcation_id, sizeof(transcation_id));
  memcpy(&pay_info[0x38], &transfer_id, sizeof(transfer_id));

  __asm {
    PUSHAD
    PUSH       0x1
    SUB        ESP,0x8
    LEA        EDX,recv_id
    LEA        ECX,pay_info
    CALL       do_confirm_addr  
    MOV        success,EAX  
    ADD        ESP,0xC
    PUSH       0x0
    LEA        ECX,pay_info
    CALL       free_pay_info_addr 
    POPAD
  }

  return success;
}

int MiscMgr::DoDownloadTask(ULONG64 msg_id) {
  int success = -1;
  int db_index = 0;
  int local_id =  DB::GetInstance().GetLocalIdByMsgId(msg_id, db_index);
  if (local_id < 1) {
    return -2;
  }

  char chat_msg[0x2D8] = {0};
  DWORD new_chat_msg_addr = base_addr_ + WX_NEW_CHAT_MSG_OFFSET;
  DWORD get_chat_mgr_addr = base_addr_ + WX_CHAT_MGR_OFFSET;
  DWORD pre_download_mgr_addr = base_addr_ + WX_GET_PRE_DOWNLOAD_MGR_OFFSET;
  DWORD push_attach_task_addr = base_addr_ + WX_PUSH_ATTACH_TASK_OFFSET;
  DWORD free_addr = base_addr_ + WX_FREE_CHAT_MSG_INSTANCE_COUNTER_OFFSET;
  DWORD get_by_local_Id_addr = base_addr_ + WX_GET_MGR_BY_PREFIX_LOCAL_ID_OFFSET;
  DWORD get_current_data_path_addr = base_addr_ + WX_GET_CURRENT_DATA_PATH_OFFSET;
  DWORD free_app_msg_info_addr = base_addr_ + WX_FREE_APP_MSG_INFO_OFFSET;
  DWORD push_thumb_task_addr = base_addr_ + WX_PUSH_THUMB_TASK_OFFSET;
  DWORD video_mgr_addr = base_addr_ + WX_VIDEO_MGR_OFFSET;
  DWORD download_video_image_addr = base_addr_ + WX_VIDEO_MGR_OFFSET;

  WeChatString current_data_path;

  __asm {
    PUSHAD
    PUSHFD
    LEA        ECX,current_data_path
    CALL       get_current_data_path_addr

    LEA        ECX,chat_msg
    CALL       new_chat_msg_addr

    CALL       get_chat_mgr_addr                                       
    PUSH       dword ptr [db_index]
    LEA        ECX,chat_msg
    PUSH       dword ptr [local_id]
    CALL       get_by_local_Id_addr               
    ADD        ESP,0x8
    POPFD
    POPAD
  }
  wstring save_path = L"";
  wstring thumb_path = L"";
  if (current_data_path.length > 0) {
    save_path += current_data_path.ptr;
    save_path += L"wxhelper";
  } else {
    return -1;
  }
 
  if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
    return -3;
  }
  DWORD type = *(DWORD *)(chat_msg + 0x38);
  wchar_t *content = *(wchar_t **)(chat_msg + 0x70);

  switch (type) {
    case 0x3: {
      save_path += L"\\image";
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      save_path = save_path +L"\\"+ to_wstring(msg_id) + L".png";
      break;
    }
    case 0x3E:
    case 0x2B: {
      save_path += L"\\video";
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      thumb_path = save_path + L"\\"+ to_wstring(msg_id) + L".jpg";
      save_path =  save_path + L"\\"+ to_wstring(msg_id) + L".mp4";
     
      break;
    }
    case 0x31: {
      save_path += L"\\file";
      wcout << save_path << endl;
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      char xml_app_msg[0xC80] = {0};
      DWORD new_app_msg_addr = base_addr_ + WX_APP_MSG_INFO_OFFSET;
      DWORD get_xml_addr = base_addr_ + WX_GET_APP_MSG_XML_OFFSET;
      WeChatString w_content(content);

      __asm {
        PUSHAD
        PUSHFD
        LEA        ECX,xml_app_msg      
        CALL       new_app_msg_addr 
        PUSH       0x1
        LEA        EAX,w_content
        PUSH       EAX       
        LEA        ECX,xml_app_msg
        CALL       get_xml_addr 
        MOV        success,EAX
        LEA        ECX,xml_app_msg
        CALL       free_app_msg_info_addr
        POPFD
        POPAD
      }
      if (success != 1) {
        return -4;
      }
      WeChatString *file_name = (WeChatString *)((DWORD)xml_app_msg + 0x44);
      save_path = save_path +L"\\" + to_wstring(msg_id) + L"_" +
                  wstring(file_name->ptr, file_name->length);
      break;
    }
    default:
      break;
  }
  WeChatString  w_save_path(save_path);
  WeChatString  w_thumb_path(thumb_path);
  int temp =1;
  memcpy(&chat_msg[0x19C], &w_thumb_path, sizeof(w_thumb_path));
  memcpy(&chat_msg[0x1B0], &w_save_path, sizeof(w_save_path));
  memcpy(&chat_msg[0x29C], &temp, sizeof(temp));
  // note： the image has been downloaded and will not be downloaded again
  // use low-level method  
  // this function does not work, need to modify chatmsg.
  // if (type == 0x3E || type == 0x2B){
  //   __asm{
  //      PUSHAD
  //      PUSHFD
  //      CALL       video_mgr_addr
  //      LEA        ECX,chat_msg
  //      PUSH       ECX
  //      MOV        ECX,EAX
  //      CALL       download_video_image_addr
  //      POPFD
  //      POPAD
  //   }
  // }

  __asm {
    PUSHAD
    PUSHFD
    CALL       pre_download_mgr_addr                                
    PUSH       0x1
    PUSH       0x0
    LEA        ECX,chat_msg
    PUSH       ECX
    MOV        ECX,EAX
    CALL       push_attach_task_addr
    MOV        success,EAX
    LEA        ECX,chat_msg
    PUSH       0x0 
    CALL       free_addr
    POPFD
    POPAD
  }

  return success;
}

int MiscMgr::GetVoice(ULONG64 msg_id, wchar_t *dir) {
  int success = -1;
  string buff = DB::GetInstance().GetVoiceBuffByMsgId(msg_id);
  if (buff.size() == 0) {
    success = 0;
    return success;
  }
  wstring save_path = wstring(dir);
  if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
    success = -2;
    return success;
  }
  save_path = save_path + L"\\" + to_wstring(msg_id) + L".amr";
  HANDLE file_handle = CreateFileW(save_path.c_str(), GENERIC_ALL, 0, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) {
    #ifdef _DEBUG
    wcout <<" save_path =" <<save_path<<endl;
    #endif
    return success;
  }
  DWORD bytes_write = 0;
  string decode = base64_decode(buff);
  WriteFile(file_handle, (LPCVOID)decode.c_str(), decode.size(), &bytes_write, 0);
  CloseHandle(file_handle);
  success = 1;
  return success;
}

int MiscMgr::GetImgByName(wchar_t* file_path,wchar_t* save_dir) {
  wstring save_path(save_dir);
  wstring orign_file_path(file_path);
  if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
    return 0;
  }

  int pos_begin = orign_file_path.find_last_of(L"\\") + 1;
  int pos_end = orign_file_path.find_last_of(L".");
  wstring file_name = orign_file_path.substr(pos_begin, pos_end - pos_begin);
  HANDLE h_origin_file =
      CreateFileW(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL, NULL);
  char buffer[BUFSIZE] = {0};
  DWORD bytes_read = 0;
  DWORD bytes_write = 0;
  unsigned char magic_head[4] = {0};
  wstring suffix;
  short key = 0;
  if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
    memcpy(magic_head, buffer, 3);
  } else {
    CloseHandle(h_origin_file);
    return 0;
  }
  if ((magic_head[0] ^ JPEG0) == (magic_head[1] ^ JPEG1)) {
    key = magic_head[0] ^ JPEG0;
    suffix = L".jpg";
  } else if ((magic_head[0] ^ PNG1) == (magic_head[1] ^ PNG2)) {
    key = magic_head[0] ^ PNG1;
    suffix = L".png";
  } else if ((magic_head[0] ^ GIF0) == (magic_head[1] ^ GIF1)) {
    key = magic_head[0] ^ GIF0;
    suffix = L".gif";
  } else if ((magic_head[0] ^ BMP0) == (magic_head[1] ^ BMP1)) {
    key = magic_head[0] ^ BMP0;
    suffix = L".bmp";
  } else {
    key = -1;
    suffix = L".dat";
  }
  wstring save_img_path = save_path + L"\\" + file_name + suffix;
  HANDLE save_img = CreateFileW(save_img_path.c_str(), GENERIC_ALL, 0, NULL,
                                 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (save_img == INVALID_HANDLE_VALUE) {
    return 0;
  }
  if (key > 0) {
    for (unsigned int i = 0; i < bytes_read; i++) {
      buffer[i]^=key;
    }
  }
  if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
    CloseHandle(h_origin_file);
    CloseHandle(save_img);
    return 0;
  }

  do {
    if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
      if (key > 0) {
        for (unsigned int i = 0; i < bytes_read; i++) {
          buffer[i] ^= key;
        }
      }
      if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
        CloseHandle(h_origin_file);
        CloseHandle(save_img);
        return 0;
      }
    }
  } while (bytes_read == BUFSIZE);
  CloseHandle(h_origin_file);
  CloseHandle(save_img);
  return 1;
}

int MiscMgr::SearchContactNetScene(wchar_t *keyword,UserInfo ** user) {
  int success = -1;
  hooks::HookSearchContact();
  hooks::DeleteUserInfoCache();
  DWORD search_contact_mgr_addr = base_addr_ + WX_SEARCH_CONTACT_MGR_OFFSET;
  DWORD search_contact_addr = base_addr_ + WX_SEARCH_CONTACT_OFFSET;
  WeChatString key(keyword);

  __asm {
		pushad;
		pushfd;
		call search_contact_mgr_addr;
		lea ebx, key;
		push ebx;
		mov ecx, eax;
		call search_contact_addr;
		popfd;
		popad;
  }
  success = 1;
  while (hooks::userinfo.error_code == 1 && hooks::user_info_flag_) {
    Sleep(20);
  }
  if (hooks::userinfo.error_code == 0) {
    while (hooks::userinfo.over == false && hooks::user_info_flag_) {
      Sleep(20);
    }
  }
  *user= &hooks::userinfo;
  LOG(INFO)<<"user:" <<user;
  return success;
}

}  // namespace wxhelper