# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from os import environ
from winreg import *
import os, sys, win32gui, win32con

class PATH_EDIT:
    _strPATH = []
    
    def __init__(self):
        self._strPATH = environ['PATH'].split(";")
    
    def getPATH(self):
        self._strPATH = environ['PATH'].split(";")
        return self._strPATH
    
    def showPATH(self):
        self.getPATH()
        for i in range(0, len(self._strPATH)):
            print('%3d  %s'%(i, self._strPATH[i]))

    def setPATH(self):
        nPATH = ''
        for i in range(0, len(self._strPATH)):
            nPATH = nPATH + self._strPATH[i] + ';'
        nPATH = nPATH[:-1]
        environ['PATH'] = nPATH
    
    def delPATH(self, idx):
        self._strPATH.remove(self._strPATH[idx])
    
    def addPATH(self, str = ''):
        if str == '':
            return
        if str.find(';') != -1:
            print('Error: invalid PATH item')
            return
        self._strPATH.append(str)

if __name__ == '__main__':
  print(sys.argv)
  pathRegEnv = r'SYSTEM\CurrentControlSet\Control\Session Manager\Environment'
  key = OpenKey(HKEY_LOCAL_MACHINE, pathRegEnv, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY)
  value, type_id = QueryValueEx(key, 'PATH')
  iList = value.split(';')
  
  n_val = ''
  for i in range(0, len(iList)):
      n_val = n_val + iList[i] + ';'
  n_val = n_val[:-1]
  ret = SetValueEx(key, 'PATH', 0, REG_EXPAND_SZ, n_val)
  print(ret)
  # win32gui.SendMessage(win32con.HWND_BROADCAST, win32con.WM_SETTINGCHANGE, 0, 'Environment')
  
  win32gui.SendMessageTimeout(win32con.HWND_BROADCAST, win32con.WM_SETTINGCHANGE, 0, 'Environment',
                       0, 5)
  #win32gui.PostMessage(win32con.HWND_BROADCAST, win32con.WM_SETTINGCHANGE, NULL,NULL);
  CloseKey(key)