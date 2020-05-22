bcmsh pvlan  芯片端口收到 无tag报文, 自动给报文加上tag 所对应的vlan

三层口也有一个 pvlan, 上来后, 内核层要剥掉硬件打上的vlantag, 然后在送到 内核interface(无vlan处理逻辑),
否则内核interface不处理802.3Q 的报文