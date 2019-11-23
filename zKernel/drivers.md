## character
### open close write read 回调
open  回调中 处理错误(如互斥化操作) 和初始化设备
close 回调中 时候释放设备
write 回调中 copy_to_user   给用户返回数据
read  回调中 copy_from_user 读取用户的数据
### 注册/注销
cls = class_create(THIS_MODULE, );
device_create();
cdev_init();
cdev_add();

cdev_del();
device_destroy();
class_destroy();
unregister_chrdev_region(,1);

## block

## misc
一种特殊的字符设备(回调是一样的), MISC_MAJOR = 10
int misc_register(struct miscdevice * misc);
int misc_deregister(struct miscdevice *misc);

## bus
总线下有设备和驱动 总线是设备和驱动的映射网
### bus register/unregister
int bus_register(struct bus_type *bus);
int bus_unregister(struct bus_type *bus);
/sys/bus 下有对应目录;
device注册注销
### driver register/unregister
int driver_register(struct device_driver *drv);
int driver_unregister(struct device_driver *drv);
/sys/bus/$bus/drivers/
### device register/unregister
int device_register(struct device *dev);
int device_unregister(struct device *dev);
/sys/bus/$bus/device/