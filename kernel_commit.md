unreferenced object 0xffff80022a7c0000 (size 10368):
  comm "iplink", pid 1313, jiffies 4294893934 (age 71392.552s)
  hex dump (first 32 bytes):
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  backtrace:
    [<ffff00000818922c>] kmalloc_order+0x44/0x58
    [<ffff0000081bc864>] __kmalloc+0x1fc/0x240
    [<ffff0000084febdc>] mvpp2_frag_alloc.isra.65+0x2c/0x38
    [<ffff0000084ff86c>] mvpp2_bm_pool_use+0x10c/0x3f0
    [<ffff0000084ffc70>] mvpp2_swf_bm_pool_init+0x120/0x1d0
    [<ffff0000084ffe1c>] mvpp2_bm_update_mtu+0xfc/0x178
    [<ffff000008502948>] mvpp2_change_mtu+0x38/0x1a8
    [<ffff0000085eed44>] dev_set_mtu+0xe4/0x1b0
    [<ffff000008618c30>] dev_ifsioc+0x330/0x3e0
    [<ffff000008618f78>] dev_ioctl+0x298/0x900
    [<ffff0000085cd734>] sock_do_ioctl+0x64/0x78
    [<ffff0000085ceca4>] sock_ioctl+0x26c/0x3b0
    [<ffff0000081ebe14>] do_vfs_ioctl+0xa4/0x9f0
    [<ffff0000081ec7d8>] SyS_ioctl+0x78/0x98
    [<ffff000008083a00>] el0_svc_naked+0x34/0x38
    [<ffffffffffffffff>] 0xffffffffffffffff

ip link set eth0 mtu 1500