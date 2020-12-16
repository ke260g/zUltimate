# 函数调用链 fchmodat
```c++
SYSCALL_DEFINE3(fchmodat, int, dfd, const char __user *, filename, umode_t, mode)
chmod_common() {
	mnt_want_write(path->mnt); // sb_start_write(m->mnt_sb);
	    ^*------------------- 加锁点1
	inode_lock(path->dentry->d_inode); // 对应日志中 lock(&ovl_i_mutex_key[depth]);
	    ^*------------------- 加锁点2
	notify_change(path->dentry) {
		inode->i_op->setattr(dentry, attr); 
		ovl_setattr(dentry, attr); {
			ovl_want_write(dentry);  // sb_start_write(m->mnt_sb);
			^*------------------- 加锁点3
			inode_lock(upperdentry->d_inode);
			^*------------------- 加锁点4
			inode_unlock(upperdentry->d_inode);
			ovl_drop_write(dentry); // sb_stop_write(m->mnt_sb);
		}		
	}
	inode_unlock(inode);
	mnt_drop_write(path->mnt); // sb_stop_write(m->mnt_sb);
}
```

# 函数调用链 fchown
```c++
SYSCALL_DEFINE3(fchown, unsigned int, fd, uid_t, user, gid_t, group) {
	// fd指向的f, file_inode(file)->i_sb, 加锁 sb->s_writer.rw_sem
	mnt_want_write_file_path(f.file) // sb_start_write(file_inode(file)->i_sb);
	          ^*----------------- 加锁点1 // 已经拿到, 日志中的 CPU0: lock(sb_writers#6);
	chown_common(&f.file->f_path) {
		inode_lock(inode); // down_write(&inode->i_rwsem);
		      ^*----------------- 加锁点2 // 拿不到锁, 日志中的 CPU0: lock(&ovl_i_mutex_key[depth]);
		notify_change(path->dentry) {
			inode->i_op->setattr(dentry, attr); 
			ovl_setattr(dentry, attr); {
				ovl_want_write(dentry);  // sb_start_write(m->mnt_sb);
				^*------------------- 加锁点3
				inode_lock(upperdentry->d_inode);
				^*------------------- 加锁点4
				inode_unlock(upperdentry->d_inode);
				ovl_drop_write(dentry); // sb_stop_write(m->mnt_sb);
			}		
		}
	}
	mnt_drop_write_file_path(f.file) // sb_start_write(file_inode(file)->i_sb);
}
```

# 在加锁点, 加上日志, 打印获取锁的对象
```log
[  188.410876] pid:[11126], comm:[sed], cpu:[1], func:[chmod_common], mnt_want_write(sb = ffff80022ed28080)
[  188.421465] pid:[11126], comm:[sed], cpu:[1], func:[chmod_common], inode_lock(inode = ffff8002131232c8)
[  188.431060] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], mmn_want_write(sb = ffff80022ec31280)
[  188.440925] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], inode_lock(inode = ffff80021318da18)
[  188.450729] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], inode_unlock(inode = ffff80021318da18)
[  188.460106] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], mnt_drop_write(sb = ffff80022ec31280)
[  188.469663] pid:[11126], comm:[sed], cpu:[1], func:[chmod_common], inode_unlock(inode = ffff8002131232c8)
[  188.479120] pid:[11126], comm:[sed], cpu:[1], func:[chmod_common], mnt_drop_write(sb = ffff80022ed28080)

[  188.488756] pid:[11126], comm:[sed], cpu:[1], func:[SYSC_fchown], mnt_want_write_file_path (sb = ffff80022ec31280)
[  188.498302] pid:[11126], comm:[sed], cpu:[1], func:[chown_common], inode_lock(inode = ffff8002131232c8)
[  188.508720]
[  188.519646] ======================================================
[  188.525852] WARNING: possible circular locking dependency detected
[  188.532059] 4.14.76-18.12.3-Marvell #22 Tainted: G           O
[  188.538177] ------------------------------------------------------
[  188.544384] sed/11126 is trying to acquire lock:
[  188.549021]  (&ovl_i_mutex_key[depth]){+.+.}, at: [<ffff0000081d62bc>] chown_common.isra.0+0x124/0x260
[  188.558383]
[  188.558383] but task is already holding lock:
[  188.564240]  (sb_writers#6){.+.+}, at: [<ffff000008200eec>] mnt_want_write_file_path+0x24/0x58
[  188.572900]
[  188.572900] which lock already depends on the new lock.
[  188.572900]
[  188.581113]
[  188.581113] the existing dependency chain (in reverse order) is:
[  188.588628]
[  188.588628] -> #1 (sb_writers#6){.+.+}:
[  188.593977]        __sb_start_write+0x138/0x1e8
[  188.598529]        mnt_want_write+0x20/0x50
[  188.602732]        ovl_want_write+0x18/0x20
[  188.606934]        ovl_setattr+0x88/0x200
[  188.610963]        notify_change+0x244/0x320
[  188.615251]        chmod_common+0xec/0x290
[  188.619365]        SyS_fchmodat+0x60/0xe0
[  188.623394]        el0_svc_naked+0x34/0x38
[  188.627507]
[  188.627507] -> #0 (&ovl_i_mutex_key[depth]){+.+.}:
[  188.633813]        lock_acquire+0x6c/0xa8
[  188.637844]        down_write+0x48/0x98
[  188.641696]        chown_common.isra.0+0x124/0x260
[  188.646507]        SyS_fchown+0xcc/0x170
[  188.650446]        el0_svc_naked+0x34/0x38
[  188.654559]
[  188.654559] other info that might help us debug this:
[  188.654559]
[  188.662598]  Possible unsafe locking scenario:
[  188.662598]
[  188.668544]        CPU0                    CPU1
[  188.673092]        ----                    ----
[  188.677641]   lock(sb_writers#6);
[  188.680975]                                lock(&ovl_i_mutex_key[depth]);
[  188.687796]                                lock(sb_writers#6);
[  188.693658]   lock(&ovl_i_mutex_key[depth]);
[  188.697949]
[  188.697949]  *** DEADLOCK ***
[  188.697949]
[  188.703896] 1 lock held by sed/11126:
[  188.707572]  #0:  (sb_writers#6){.+.+}, at: [<ffff000008200eec>] mnt_want_write_file_path+0x24/0x58
[  188.716669]
[  188.716669] stack backtrace:
[  188.721047] CPU: 1 PID: 11126 Comm: sed Tainted: G           O    4.14.76-18.12.3-Marvell #22
[  188.729608] Hardware name: Marvell Armada 7040 development board default (A) setup (DT)
[  188.737645] Call trace:
[  188.740104] [<ffff000008088498>] dump_backtrace+0x0/0x420
[  188.745528] [<ffff0000080888cc>] show_stack+0x14/0x20
[  188.750601] [<ffff00000878e5e0>] dump_stack+0xbc/0xf4
[  188.755675] [<ffff0000080f7d58>] print_circular_bug.isra.17+0x1d0/0x2e0
[  188.762319] [<ffff0000080fae4c>] __lock_acquire+0x18ac/0x1b78
[  188.768090] [<ffff0000080fb88c>] lock_acquire+0x6c/0xa8
[  188.773339] [<ffff0000087a6fb8>] down_write+0x48/0x98
[  188.778412] [<ffff0000081d62bc>] chown_common.isra.0+0x124/0x260
[  188.784445] [<ffff0000081d72e4>] SyS_fchown+0xcc/0x170
[  188.789606] Exception stack(0xffff80020f023ec0 to 0xffff80020f024000)
[  188.796076] 3ec0: 0000000000000003 0000000000000000 0000000000000000 0000000000000000
[  188.803940] 3ee0: 00000000005f6000 00000000005f63b0 0000000000000131 0000000000000231
[  188.811805] 3f00: 0000000000000037 0000000000000000 0000ffffd5947eb0 0000000000000880
[  188.819668] 3f20: 000000000000004c 0000000000001095 0000000000000044 0000000000000018
[  188.827532] 3f40: 0000000000554e47 000000000000000a 0000000000000001 0000ffffd5948450
[  188.835396] 3f60: 00000000005fa000 00000000005fac18 00000000005f6000 0000000000000001
[  188.843259] 3f80: 00000000005d9c38 00000000005d2621 0000000000000003 00000000005fa000
[  188.851122] 3fa0: 0000000000000000 0000ffffd59482e0 0000000000560cf8 0000ffffd5948120
[  188.858986] 3fc0: 0000000000401bf8 0000000000000000 0000000000000003 0000000000000037
[  188.866849] 3fe0: 0000000000000000 0000000000000000 0000000000000000 0000000000000000
[  188.874714] [<ffff000008083600>] el0_svc_naked+0x34/0x38
[  188.880104] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], mmn_want_write(sb  f=fff80022ec31280)
[  188.880113] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], inode_lock(inode = ffff80021318da18)
[  188.889628] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], inode_unlock(inode = ffff80021318da18)
[  188.899021] pid:[11126], comm:[sed], cpu:[1], func:[ovl_setattr], mnt_drop_write(sb = ffff80022ec31280)
[  188.908555] pid:[11126], comm:[sed], cpu:[1], func:[chown_common], inode_unlock(inode = ffff8002131232c8)
[  188.917998] pid:[11126], comm:[sed], cpu:[1], func:[SYSC_fchown], mnt_drop_write_file_path(sb = ffff80022ec31280)
```

# 最终结论
根据加锁对象的指针可以知
1. overlayfs 中 fchmodat 依次加锁A B **C D**; 依次解锁 **D C** B A
	+ (其中**C D** 锁是 overlayfs 驱动层的)
2. overlayfs 中 fchown   依次加锁C B **C D**; 依次解锁 **D C** B C
	+ (其中**C D** 锁是 overlayfs 驱动层的)

> 技术上, overlayfs 驱动层 为 super_block 加锁时, 复用了上层文件系统的锁,
> 即  ovl_setattr 中 `ovl_want_write(dentry);` 函数,
> 最终导致 fchmodat 加锁 A B C, fchown 加锁 C B.
> 多进程同时操作一个文件, 可能就有问题

3. ovl_want_write 具体复用逻辑
```c++
int ovl_want_write(struct dentry *dentry)
{
	struct ovl_fs *ofs = dentry->d_sb->s_fs_info;
	return mnt_want_write(ofs->upper_mnt);
}
```
