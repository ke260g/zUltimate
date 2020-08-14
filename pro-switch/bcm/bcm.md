BCM_HURRICANE2_SUPPORT
BCM_XXX_SUPPORT 在 sdk-$version/include/soc/defs.h 中

BCM_$芯片系列ID = `bcmsh soc 获取`; `Chip字段, Driver字段`


dcb_type 在  /proc/bcm/knet/debug grep出来
dcb_type 的定义
+ soc/shared/dcbformats/type$type_id.h
+ src/soc/common/dcb.c 这里设置芯片的dcb_type



```c++
dcb_op_t dcb30_op = {
    30,
    sizeof(dcb20_t),
}

soc_dcb_unit_init(int unit)
{
    soc_control_t       *soc;
    soc->dcb = &dcb30_op;
};
```

获取unit
```c++
sinfo->dev_no
```

bcmsh version
bcmsh soc


三层口查看
bcmsh l3 intf show
bcmsh l3 egress show
bcmsh l3 defip show