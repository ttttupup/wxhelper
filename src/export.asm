;#################################################################

.code


;#################################################################


;#################################################################

;检测wechat登录状态
;param: get_account_service_addr  函数地址

;#################################################################
_GetAccountService PROC,
      get_account_service_addr:QWORD                    ; 函数地址
            sub   rsp,28h
            call  rcx
            add   rsp,28h
            ret
_GetAccountService ENDP

;#################################################################

;获取wechat数据保存路径
;param: addr  函数地址
;return：路径地址

;#################################################################
_GetDataSavePath PROC,
      get_data_path_addr:QWORD,                         ; 函数地址
      out_path:QWORD                                    ; 输出
            sub   rsp,40h
            mov   rax,rcx
            mov   rcx,rdx
            call  rax
            add   rsp,40h
            ret
_GetDataSavePath ENDP


;#################################################################

;获取wechat当前数据保存路径
;param: addr  函数地址
;return：路径地址

;#################################################################
_GetCurrentDataPath PROC,
      get_current_path_addr:         QWORD,             ; 函数地址
      out_path:                QWORD                    ; 输出
            sub   rsp,28h
            mov   rax,rcx
            mov   rcx,rdx
            call  rax
            add   rsp,28h
            ret
_GetCurrentDataPath ENDP


END