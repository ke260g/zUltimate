[TOC]
vfs 是基于 unix 源码实现过来的
1. super_block: 文件系统的元数据 = 文件系统类型/大小/状态/其他元数据相关的信息(元元数据)
    + 在储存设备中, 并序列化到内核
2. inode: 文件相关的元数据 = 所有者(用户/组)) 访问时间 文件类型(ls -l 首字母), 但不含名字
    + 在存储设备中, 并序列化到内核
3. dentry: path and inode 的对应关系, directory 下的文件列表
    + 只存在于内核, 由 inode 构造出来
4. file: 进程打开的文件对象
    + 只存在于内核

# 1. struct super_block
```c++
struct super_block {
	struct list_head	s_list;              // 连接 super_blocks
	dev_t			s_dev;		             // 指向 设备信息
    struct file_system_type	*s_type;         // 文件系统类型
    struct dentry		*s_root;             // 文件系统的跟目录
    const struct super_operations	*s_op;   // vfs 抽象方法, 由文件系统实现
};
static LIST_HEAD(super_blocks); // 连接 super_block->s_list

struct vfsmount {
	struct dentry *mnt_root;	/* root of the mounted tree */
	struct super_block *mnt_sb;	/* pointer to superblock */
	int mnt_flags;
} __randomize_layout;
```
+ `super_block` 文件系统的metadata
    + 在存储设备分区开头
    + 挂载之后读取磁盘数据构造对象
+ `super_blocks` 索引所有文件系统的链表
+ `vfsmount->mnt_sb` 挂载节点的文件系统


# 2. struct file_system_type
```c++
struct file_system_type {
	const char *name;
    int fs_flags;
    /* 挂载文件系统的回调, 回调中创建 superblock
     * 伪过程: sb = sget(); return sb->s_root; */
    struct dentry *(*mount) (struct file_system_type *, int, const char *, void *);
    /* 卸载文件系统的回调, 回调中销毁 superblock */
	void (*kill_sb) (struct super_block *);
    /* 连接 全局链表 file_systems */
    struct file_system_type * next;
};
static struct file_system_type *file_systems;
static DEFINE_RWLOCK(file_systems_lock); // 操作锁
```
+ `file_system_type` 文件系统类型, 主要实现创建和销毁方法
    1. `register_filesystem()`,   传入文件系统类型, 加入到 file_systems 中
    2. `unregister_filesystem()`, 传入文件系统类型, 从 file_systems 中删除
    3. SYSCALL_DEFINE5(mount > do_mount > do_new_mount > 
       vfs_kern_mount > mount_fs > mount方法
    4. SYSCALL_DEFINE2(mount > do_umount > 
       namespace_unlock > mntput > mntput_no_expire >
       deactivate_super > deactivate_locked_super > kill_sb方法

# 3. struct super_operations
```c++
struct super_operations {
   	struct inode *(*alloc_inode)(struct super_block *sb);
	void (*destroy_inode)(struct inode *);
   	void (*dirty_inode) (struct inode *, int flags);
	int (*write_inode) (struct inode *, struct writeback_control *wbc);
	int (*drop_inode) (struct inode *);
	void (*evict_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_fs) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*umount_begin) (struct super_block *);
	int (*show_options)(struct seq_file *, struct dentry *);
	int (*show_devname)(struct seq_file *, struct dentry *);
	int (*show_path)(struct seq_file *, struct dentry *);
	int (*show_stats)(struct seq_file *, struct dentry *);
	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
	long (*nr_cached_objects)(struct super_block *, int);
	long (*free_cached_objects)(struct super_block *, long, int);
};
```
+ `super_operations` 文件系统方法, 例如
    + write  调用: sb->s_op->write_inode
    + statfs 调用: sb->s_op->statfs


# 4. struct inode
```c++
struct inode {
	umode_t			i_mode;           // 权限
	unsigned short		i_opflags;    // open 的 标记
	kuid_t			i_uid;            // 拥有者
	kgid_t			i_gid;            // 所属组
	unsigned int		i_flags;
	const struct inode_operations	*i_op;
	struct super_block	*i_sb;
	dev_t			i_rdev;           // 设备
	loff_t			i_size;           // 文件大小
	const struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
	struct file_lock	*i_flock;     // 文件锁
	struct address_space	i_data;   // 数据
};
```
## 4.1 inode->i_fop
1. 由文件系统实现, 挂载时赋值 s_root, open文件时其他inode继承s_root的
2. 例子
	1. 如 ext4fs:  ext4_file_operations / ext4_dir_operations
	2. 如 xfs:     xfs_file_operations / xfs_dir_file_operations
## 4.2 inode->i_op
1. 由文件系统实现, 挂载时赋值 s_root, open文件时其他inode继承s_root的
2. 例子
	1. 如 ext4fs: ext4_file_inode_operations / ext4_dir_inode_operations
	2. 如 xfs:    xfs_inode_operations / xfs_dir_inode_operations

# 5. struct inode_operations
```c++
struct inode_operations {
	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
	void * (*follow_link) (struct dentry *, struct nameidata *);
	int (*permission) (struct inode *, int);
	struct posix_acl * (*get_acl)(struct inode *, int);

	int (*readlink) (struct dentry *, char __user *,int);
	void (*put_link) (struct dentry *, struct nameidata *, void *);

	int (*create) (struct inode *,struct dentry *, umode_t, bool);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct inode *,struct dentry *,const char *);
	int (*mkdir) (struct inode *,struct dentry *,umode_t);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);
	int (*rename) (struct inode *, struct dentry *,
			struct inode *, struct dentry *);
	int (*rename2) (struct inode *, struct dentry *,
			struct inode *, struct dentry *, unsigned int);
	int (*setattr) (struct dentry *, struct iattr *);
	int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
	int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);
	ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
	ssize_t (*listxattr) (struct dentry *, char *, size_t);
	int (*removexattr) (struct dentry *, const char *);
	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
		      u64 len);
	int (*update_time)(struct inode *, struct timespec *, int);
	int (*atomic_open)(struct inode *, struct dentry *,
			   struct file *, unsigned open_flag,
			   umode_t create_mode, int *opened);
	int (*tmpfile) (struct inode *, struct dentry *, umode_t);
	int (*set_acl)(struct inode *, struct posix_acl *, int);
} ____cacheline_aligned;
```
1. 保存在储存设备上, 每个文件的 metadata
2. vfs 操作单个文件, 底层操作对象是 inode
    1.  create 方法: `open(O_CREAT)`
    2. setattr 方法: `chmod()` / `chown()`
    3.  rename 芳芳: `rename()`
3. 写延时的策略, 在 inode 层完成

# 6. struct dentry
```c++
struct dentry {
    struct hlist_bl_node d_hash;/* dcache 缓存器的 dentry_hashtable 节点 */
	struct dentry *d_parent;	/* 上级节点 */
	struct inode *d_inode;		/* inode 节点 */
    unsigned char d_iname[DNAME_INLINE_LEN];	/* 文件名 */
    const struct dentry_operations *d_op;
    struct super_block *d_sb;	/* 该文件的 super_block */
	struct list_head d_lru;		/* LRU list */
	struct list_head d_child;	/* 连接上级节点的 所有下级节点的链表 */
	struct list_head d_subdirs;	/* 该节点的下级节点 */
};
```
1. 映射 pathname 与 inode 间的关系
    1. d_inode
    2. d_iname
2. 实现 文件系统的树状层次结构 (即 "硬链接" "缓存" "多级目录" 等特性)
3. 每个文件系统有一个根节点(sb->s_root), 其上级节点为空( d_parent == null )
4. 根节点为进行树形结构的查找入口
## 生命周期 (访问文件时才创建, 并缓存起来)
有3个状态
1. used:   与inode关联，正在被引用，不能被损坏和丢弃
2. unused: 与inode关联，没有被引用, 处于缓存状态 (可以丢弃, 等待inode被复用)
3. negative: 无inode关联, 相当于是一个无效的路径

## dentry_cache (缓存器) (fs/dcache.c)
```c++
static struct kmem_cache *dentry_cache __read_mostly;
static struct hlist_bl_head *dentry_hashtable __read_mostly;
```
1. used dentries 链表: 记录 used 的 dentry
2. lru 双向链表: 维护unused和negative状态的dentry对象
    1. 当节点不再被引用, 则插入到 lru 链表中
    2. 头插入法, 离头部越近就是最近访问过的
    3. 当需要销毁 dentry 时(超时 or 超出大小), 
    4. 从队列尾部销毁最旧的 dentry
3. `dentry_hashtable` hash 表快速查询一个给定的路径到dentry对象

# 7. struct dentry_operations
```c++
struct dentry_operations {
	int (*d_revalidate)(struct dentry *, unsigned int);
	int (*d_weak_revalidate)(struct dentry *, unsigned int);
	int (*d_hash)(const struct dentry *, struct qstr *);
	int (*d_compare)(const struct dentry *, const struct dentry *,
			unsigned int, const char *, const struct qstr *);
	int (*d_delete)(const struct dentry *);
	void (*d_release)(struct dentry *);
	void (*d_prune)(struct dentry *);
	void (*d_iput)(struct dentry *, struct inode *);
	char *(*d_dname)(struct dentry *, char *, int);
	struct vfsmount *(*d_automount)(struct path *);
	int (*d_manage)(struct dentry *, bool);
} ____cacheline_aligned;
```

# 8. struct file
```c++
struct file {
	struct path		f_path;
#define f_dentry	f_path.dentry
	struct inode		*f_inode;	/* cached value */
	const struct file_operations	*f_op;
	spinlock_t		f_lock;      // 操作锁
	atomic_long_t		f_count; // 文件对象的引用计数
	unsigned int 		f_flags;
	fmode_t			f_mode;      // 权限
	struct mutex		f_pos_lock; // 读写游标锁
	loff_t			f_pos;          // 读写游标
};
```
1. 文件对象, 进程中 fd 对应的文件对象
2. 磁盘文件的 file->f_op 在 open 时, 指向 inode->i_fop
	+ do_dentry_open 函数中
	  `inode = f->f_inode = f->f_path.dentry->d_inode;`
	  `f->f_op = fops_get(inode->i_fop);`

# 9. file_operations
```c++
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	int (*iterate) (struct file *, struct dir_context *);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
	int (*aio_fsync) (struct kiocb *, int datasync);
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	int (*setlease)(struct file *, long, struct file_lock **);
	long (*fallocate)(struct file *file, int mode, loff_t offset, loff_t len);
	int (*show_fdinfo)(struct seq_file *m, struct file *f);
};
```
1. 文件对象的操作方法
