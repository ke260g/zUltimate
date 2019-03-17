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
