内核 bonding
bond口列表      = /sys/class/net/bonding_masters            
bond口平衡模式  = /sys/class/net/$interface/bonding/mode []
bond口的配置    = /proc/net/bonding/$interface
bond口子接口    = /sys/class/net/$interface/bonding/slaves
    echo "+subinterface" > slaves
    echo "-subinterface" > slaves
bond口子接口    = /sys/class/net/$interface/bonding/active_slave
bond口子接口    = /sys/class/net/$interface/slave_$slave
bond口子接口    = /sys/class/net/$interface/lower_$slave