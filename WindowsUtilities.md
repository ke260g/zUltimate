network:
    postman laragon heidisql
    vncviewer teamviewer fiezilla
    wireshark
    v2ray
    netlimiter fdm5
devel:
    spyder
    HxD
    idaq64
    kdiff3

shell: 
    MobaXterm PAC Putty XShell
PAC patch
curl -s https://packagecloud.io/install/repositories/asbru-cm/asbru-cm/script.deb.sh | sudo bash
sudo apt install libgnome2-vte-perl
find /path/to/pac -name "Vte.so" -exec rm {} +

windows 任务栏挂掉了
cmd /c wmic process where name="explorer.exe" delete&explorer reg delete HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\StuckRects2 /f&taskkill /f /im explorer.exe&explorer