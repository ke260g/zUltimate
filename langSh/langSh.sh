#!/bin/sh'

# 字符串长度 ----------------------
s="I am a amazing string"
echo string length is ${#s}
echo string lenght is `echo -n "$s" | wc -c`
# 字符串长度 end ------------------

# 数组 ----------------------------
array=(violin cello piano)
# 1. 获取数组成员个数
echo array size is ${#array[*]}
# 2. 获取数组指定成员
echo array[1] item is [ ${array[1]} ] 
# 3. 获取数组指定成员的 字符串长度
echo array[1] size is ${#array[1]}
# 4. 遍历数组
for item in ${array[@]}; do
    echo [loop array with "for"]: $item
done
# 数组 end -----------------------

# 数值计算
echo '$((1 + 1))' == $((1 + 1)) # it will be `2'
echo '$((2 - 1))' == $((2 - 1)) # it will be `1'
echo '$((2 * 2))' == $((2 * 2)) # it will be `4'
echo '$((6 / 3))' == $((6 / 3)) # it will be `2'

# 数值比较 ---------------------------------
# [[ ]],  [ ] 中表达式必须与中括号 用空格隔开
# 双中括号可以用比较符 ( == != > < >= <= ) 和逻辑符 ( || && )
if [[ 2 > 1 ]]; then echo yes; fi          # 直接比较
if [[ $((4 / 2)) > 1 ]]; then echo yes; fi # 计算比较
if [[ 1024 == 1024 ]]; then echo yes; fi # 布尔表达式

# 单中括号只能使用比较参 -eq -ne -gt -lt -ge -le 和逻辑参 ( -o -a )
# -o 表示 ||
# -a 表示 &&
if [ 2 -gt 1 ]; then echo yes; fi
if [ $((4 / 2)) -gt 1 ]; then echo yes; fi
if [ 2048 -ne 1024 ]; then echo yes; fi
# 数值比较 ---------------------------------