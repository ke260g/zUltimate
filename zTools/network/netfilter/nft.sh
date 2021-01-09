# https://wiki.nftables.org/wiki-nftables/index.php/Quick_reference-nftables_in_10_minutes
# https://wiki.nftables.org/wiki-nftables/index.php/Main_Page

# tables: allowed to create by user
# chains: allowed to create by user
# family: ip inet bridge netdev arp ip6       (refers tables)
# type  : filter route nat                    (refers chains)
# hook  : prerouting input forward output
# policy: accept drop queue continue return

# 1. 构建table (如果只有这句; "filter"表 是没有任何意义的)
nft add table bridge filter
# 2. 使能table (使得 自建的filter表->自建的forward链 与 内核的filter->forward 实现映射)
#              (使得 自建的filter表->自建的ouptut 链 与 内核的filter->output  实现映射)
nft add chain bridge filter forward { type filter hook forward priority 0\; policy drop\; }
nft add chain bridge filter output  { type filter hook output   priority 0\; }

# 3. 允许部分转发
nft add rule bridge filter forward meta iifname \"ge*\" meta oifname \"ext1\" accept
nft add rule bridge filter forward meta iifname \"ext1\" meta oifname \"ge*\" accept
nft add rule bridge filter forward meta iifname \"ge*\" meta oifname \"ext2\" accept
nft add rule bridge filter forward meta iifname \"ext2\" meta oifname \"ge*\" accept

# 使环路
nft add rule bridge filter forward meta iifname \"ext*\" meta oifname \"ext*\" accept
nft flush chain bridge filter forward

# 删除 bridge->filter
nft delete table bridge filter

# 删除 规则
nft delete rule [<family>] <table> <chain> [handle <handle>]
nft delete rule inet filter forward  meta iifname \"ext2\" meta oifname \"ge*\" accept