内核驱动Makefile
```Makefile
CONFIG_MODULE_SIG=n # 避免签名问题 
obj-m:=hello.o
hello-objs:=helloworld.o
 
KERNEL_ROOT=/lib/modules/`uname -r`/build/
CUR_DIR=$(shell pwd

default:
        $(MAKE) -C $(KERNEL_ROOT) M=$(CUR_DIR) modules
 
clean:
        rm -rf *.o *.ko *.mod.c .*.cmd .*.cmd.c modules.* Module.* .tmp_versions
```

make -C $KENNEL_DIRECTORY M=$MODULE_DIRECTORY modules