#if 0 // [top]
1 major/minor number (Documentation/devices.txt)
2 alloc/init/add/del methods
3 open/release/read/write's implement policy
#endif

MAJOR(dev_t dev);
MINOR(dev_t dev);
MKDEV(int major, int minor);

// register   MKDEV(MAJOR(n), MINOR(n));  n 的值域 [from, from + count)
int register_chrdev_region(dev_t from, unsigned count, const char *name);
// unregister MKDEV(MAJOR(n), MINOR(n));  n 的值域 [from, from + count)
void unregister_chrdev_region(dev_t from, unsigned count);

// dynamic alloc; 输出dev 作为major-number
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name);
// release; unregister_chrdev_region底层也是调用这个接口
void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, const char *name);

每个major的minor最大容量是固定的;
每个major可以被多次注册minor; 前提是多次不同 minor 值域没有交叉

struct cdev *cdev_alloc(void);
void cdev_init(struct cdev *cdev, const struct file_operations *fops);
// 加入内核容器; @dev 即 majar-number
int cdev_add(struct cdev *p, dev_t dev, unsigned count);
// 移出内核容器;
void cdev_del(struct cdev *p);

/**
 * @note open  task
 *       1. Check  for  device-specific  errors  (such  as  device-not-ready  or  similar  hardwareproblems)•  
 *       2. Initialize the device if it is being opened for the first time•
 *       3. Update the filp->f_op pointer, if necessary•  
 *       4. Allocate and fill any data structure to be put in filp->private_data
 *          Or associate the global type cdev-object to it. For example:
 *          struct my_dev *dev = container_of(inode->i_cdev, struct my_dev, cdev);
 *          filp->private_data = dev; // prepare for other methods
 */
int (*open)(struct inode *inode, struct file *filp);
/**
 * @note release task
 *       1. Deallocate anything thatopen allocated infilp->private_data
 *       2. Shut down the device on last close
 *       refer-count
 *       fork(), dup() 不会多次调用open; 内核维护 refer-count, 使加一
 *       syscall close() refer-count 减一, 只有当 refer-count 为零 release()
 *       即 每一次open 只会对应一次release.
 */
int (*release)(struct inode *inode, struct file *filp);
/**
 * @note 1. use copy_from_user() to get data from user
 *       2. update *offp, which will be propagated to filp->f_pos
 */
ssize_t (*read)(struct file *filp, char __user *buff, size_t, loff_t *offp);
/**
 * @note 1. use copy_from_user() to put data to user
 *       2. update *offp, which will be propagated to filp->f_pos
 */
ssize_t (*write)(struct file *filp, const char __user *buff, size_t, loff_t *offp);

// note
// 1. inode->i_cdev is point to the cdev, added by cdev_add(), which will be shared by other open.
// 2. struct my_dev *dev = container_of(inode->i_cdev, struct my_dev, cdev); // cdev is a member of my_dev