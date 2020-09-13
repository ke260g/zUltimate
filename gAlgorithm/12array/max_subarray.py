#!/usr/bin/env python
# coding: utf-8
from functools import reduce
import random
import math

def divMethod(s):
    pass

def analyzeMethod(a):
    curr_sum = 0
    p = [0] # 前i项和
    for i in a:
        p.append(p[-1] + i)

    min_index = 0
    max_begin, max_end, max_value = 0, 0, 0

    for i in range(len(p)):
        if p[i] - p[min_index] > max_value:
            max_begin = min_index
            max_end = i
            max_value = p[i] - p[min_index]
        if p[i] < p[min_index]:
            min_index = i
    return a[max_begin:max_end].copy()


def dpMethod(a):
    curr_begin, curr_end, curr_value = 0, 0, a[0]
    max_begin, max_end, max_value = curr_begin, curr_end, curr_value

    for i in range(1, len(a)):
        if curr_value < 0: # a[i] 成为当前待定序列的头
            curr_begin, curr_end = i, i
            curr_value = a[i]
        else:              # a[i] 成为当前待定序列的尾
            curr_end = i
            curr_value += a[i]
        if curr_value > max_value:
            max_begin, max_end = curr_begin, curr_end
            max_value = curr_value
    return a[max_begin:max_end+1].copy()

a = [1, -2, 3, 10, -4, 7, 2, -5 ]
r = [ 3, 10, -4, 7, 2 ] # correct result
print('src: ', a)
print('ans: ', r)

units = []
units.append(a)
units.append([ round(random.random()*10 - 5) for x in range(20)])
for u in units:
    print('src: ', u)
    for m in [ dpMethod, analyzeMethod ]:
        retval = m(u)
        print(retval, sum(retval), m)
