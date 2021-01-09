# proxychains 有两个版本; 新版本是 proxychains-ng; 简单易用
apt install proxychains4 # 这个是新版本 https://github.com/rofl0r/proxychains-ng
proxychains4 -f /path/to/proxychains.conf # 配置文件 a)
vim ./proxychains.conf                    # 配置文件 b)
vim $(HOME)/.proxychains/proxychains.conf # 配置文件 c)
vim $(sysconfdir)/proxychains.conf        # 配置文件 d)

apt install torsocks
apt install tsocks # 这玩意有BUG; socks5 版本号错误转义; 没人维护了