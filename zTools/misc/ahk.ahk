; 密码
;; 研发
#!^o::
Send, sangfor123sunwns{Enter}
return

#!^l::
Send, sangfor123
return

;; 默认
#!^i::
Send, adminsunwns{Enter}
return

#!^k::
Send, admin
return

;; 测试
#!^u::
Send, wnst12345sunwns{Enter}
return

#!^j::
Send, wnst12345
return

;; 无线
#!^y::
Send, qwer1234sunwns{Enter}
return

#!^h::
Send, qwer1234
return

; 常用差错命令
#!^q::
Send, netstat -apn | grep ESTABLISHED| grep 7070 | grep tunnel_proxyd {Enter}
return

#!^w::
Send, netstat -apn | grep ESTABLISHED| grep 5000 | grep sc_proxyd {Enter}
return

#!^e::
Send, appinit_cli info | awk '$2>1 || $1<0 {print}' {Enter}
return