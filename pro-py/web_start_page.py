# -*- coding: utf-8 -*-
from winreg import *
import os, sys, win32gui, win32con

default_url=r'http://www.baidu.com'

dicRegDirVar =  [
        [HKEY_LOCAL_MACHINE, r'Software\Microsoft\Internet Explorer\Main', 'Start Page'],
        [HKEY_LOCAL_MACHINE, r'Software\Microsoft\Internet Explorer\Main', 'First Home Page'],
        [HKEY_LOCAL_MACHINE, r'Software\Microsoft\Internet Explorer\Main', 'Default_Page_URL'],
        [HKEY_CURRENT_USER, r'Software\Microsoft\Internet Explorer\Main', 'Start Page'],
        [HKEY_CURRENT_USER, r'Software\Microsoft\Internet Explorer\Main', 'First Home Page'],
        [HKEY_CURRENT_USER, r'Software\Microsoft\Internet Explorer\Main', 'Default_Page_URL']
        ]

for i in range(0, len(dicRegDirVar)):
    it = dicRegDirVar[i]
    key = OpenKey(it[0], it[1], 0, KEY_ALL_ACCESS)

    value, type_id = QueryValueEx(key, it[2])
    print('old url is %s'%(value))
    SetValueEx(key, it[2], 0, REG_SZ, default_url)

    win32gui.SendMessageTimeout(win32con.HWND_BROADCAST,
            win32con.WM_SETTINGCHANGE, 0, '', 0, 5)
    value, type_id = QueryValueEx(key, it[2])
    print('new url set to %s'%(value))
    CloseKey(key)
