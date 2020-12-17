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

# 字符串比较 -------------------------------
# 1. 空字符串
s=""
if [ -z $s ]; then string is empty; fi 
s="string"
# 2. 非空字符串
if [ -n $s ]; then string is not empty; fi

# 3. 字符串比较, 不能使用 -eq, -ne, 因为这两个只能用于 整形数值
s1="string_mini"
s2="string_mini"
s3="string_plus"
if [ $s1  = $s2 ]; then s1 and s2 are equal; fi      # 相等比较
if [ $s1 != $s3 ]; then s1 is not equal with s3; fi  # 不等比较
# 字符串比较 -------------------------------

# case 语句 -------------------------------
case $param in
    param_a)
        echo param_a
    ;;
    param_b)
        echo param_b
    ;;
    *)
        echo unknown param
    ;;
esac
# case 语句 -------------------------------

# for 累加
for((i=1;i<=10;i++));do echo $i; done # PS: 原始sh不支持, 需要bash