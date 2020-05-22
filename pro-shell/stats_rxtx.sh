interface=eth0
interface=vlan4095
timer_period=5.0 # 必须用浮点数表示, 用于awk的浮点数运算

rx_bytes_r=0
tx_bytes_r=0
rx_packets_r=0
tx_packets_r=0

is_init=0
while true; do
    # 原始数据
    raw_stats=$(ifconfig $interface)

    # 提前数据
    rx_bytes=$(echo $raw_stats | grep "RX bytes:" | sed 's|.*RX bytes:\([0-9]*\) .*$|\1|')
    tx_bytes=$(echo $raw_stats | grep "TX bytes:" | sed 's|.*TX bytes:\([0-9]*\) .*$|\1|')
    rx_packets=$(echo $raw_stats | grep "RX packets:" | sed 's|.*RX packets:\([0-9]*\) .*$|\1|')
    tx_packets=$(echo $raw_stats | grep "TX packets:" | sed 's|.*TX packets:\([0-9]*\) .*$|\1|')

    #  统计
    if [ $is_init == 0 ]; then
        is_init=1 # 第一次不统计
    else
        
        echo rx rate bytes = $(awk "BEGIN{print ($rx_bytes - $rx_bytes_r) / $timer_period  }") / s
        echo tx rate bytes = $(awk "BEGIN{print ($tx_bytes - $tx_bytes_r) / $timer_period  }") / s
        echo rx rate packets = $(awk "BEGIN{print ($rx_packets - $rx_packets_r) / $timer_period  }") / s
        echo tx rate packets = $(awk "BEGIN{print ($tx_packets - $tx_packets_r) / $timer_period  }") / s
        echo
    fi

    # 更新
    rx_bytes_r=$rx_bytes
    tx_bytes_r=$tx_bytes
    rx_packets_r=$rx_packets
    tx_packets_r=$tx_packets

    sleep $timer_period
done