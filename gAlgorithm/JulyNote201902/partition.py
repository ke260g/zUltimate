import random
# 三元数组分割,
#  input: 0121200011
# output: 0000111122
#
# 遍历完成后
# 区间: [0, begin)     ==> labels[0]
# 区间: (end, len - 1] ==> labels[2]
# a[begin] == labels[1]
# a[end] != labels[2]
def partitionTri(a, labels):
    if len(labels) != 3:
        return;
    begin, curr, end = 0, 0, len(a)-1
    while curr <= end: # 存在 a[end] == labels[0] 的可能
        if a[curr] == labels[2]:
            a[curr], a[end] = a[end], a[curr]
            end -= 1
        elif a[curr] == labels[1]:
            curr += 1
        elif a[curr] == labels[0]:
            a[curr], a[begin] = a[begin], a[curr]
            begin += 1
            curr += 1

# 正负partition
def partitionBi(a):
    begin, end = 0, len(a)-1
    while begin < end:
        if a[begin] > 0:
            a[end], a[begin] = a[begin], a[end]
            end -= 1
        else:
            begin += 1

# 三元partition
for a in [ [random.randint(0, 2) for i in range(20)] for i in range(5) ]:
    print('src: ', a)
    partitionTri(a, [0, 1, 2])
    print('dst: ', a)

# 二元partition 正负partition
for a in [ [random.randint(-10, 10) for i in range(10)] for i in range(5) ]:
    while True:
        try:
            a.remove(0)
        except:
            break
    print('src: ', a)
    partitionBi(a)
    print('dst: ', a)
