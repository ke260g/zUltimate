def raw_match(string, pattern):
    i = 0 # string index
    j = 0 # pattern index
    ls = len(string)
    lp = len(pattern)
    while i < ls and j < lp and i + j < ls:
        if string[i + j] == pattern[j]:
            j += 1
        else:
            i += 1
            j = 0

    if i + j <= ls and j == lp:
        return j
    return -1

def gen_next1(s):
    # 算法1: r[0] = -1
    # 计算next时 next[0] = -1 标记递归出口
    r = [ -1 for i in range(len(s))]
    i = 1
    k = -1
    while i < len(s):
        if k == -1 or s[i-1] == s[k]:
            r[i] = k + 1
            k += 1
            i += 1
        else:
            k = r[k]
    return r

def gen_next2(s):
    # 算法2: r[0] = 0, r[1] = 0
    r = [ 0 for i in range(len(s))]
    i = 2
    k = 0
    while i < len(s):
        if s[i - 1] == s[k]:
            r[i] = k + 1
            k += 1
            i += 1
        elif k == 0:
            r[i] = 0
            i += 1
        else:
            k = r[k]
    return r

def gen_next3(s):
    # 算法2a: r[0] = 0, r[1] = 0
    # 基于算法2
    # 递归等于 r[i] = r[...r[r[k]]]
    # 优化 aaabaaab 中找 aaaa的情况
    r = [ 0 for i in range(len(s))]
    i = 2
    k = 0
    while i < len(s):
        if s[i - 1] == s[k]:
            if s[i] == s[k + 1]:
                r[i] = r[k]
            else:
                r[i] = k + 1
                k += 1
                r[i] = k
            i += 1
        elif k == 0:
            r[i] = 0
            i += 1
        else:
            k = r[k]
    return r

def kmp_match(string, pattern):
    # step_1 计算 next数组 O(n)
    # k表示 上一个index 匹配的前后缀长度
    # 即k = r[i-1], 且可递归

    kmp_next = gen_next3(pattern)

    # step_2 匹配
    i = 0 # string index
    j = 0 # pattern index
    ls = len(string)
    lp = len(pattern)

    i_res_l = []
    j_res_l = []
    while i < ls and j < lp:
        i_res_l.append(i)
        j_res_l.append(j)
        '''
        # 算法1
        if j == -1 or string[i] == pattern[j]:
            j += 1
            i += 1 # 关键语句 使得时间为 O(n)
        else:
            j = kmp_next[j]
        '''

        # 算法2, 更优: 减少j = 0时的耗时
        if string[i] == pattern[j]:
            j += 1
            i += 1 # 关键语句 使得时间为 O(n)
        elif j == 0:
            i += 1
        else:
            j = kmp_next[j]

    print('kmp_next', kmp_next)
    print(string, len(string))
    print(pattern)
    print(i_res_l, len(i_res_l))
    print(j_res_l, len(j_res_l))
    if j == lp:
        return i - j
    return -1

s = 'abaabcaba'
#s = 'aacaacb'
retval = kmp_match('', 'abaabcaba')
print(retval)
print('---------------------------------')
retval = kmp_match('abfabcabaabab', 'abc')
print(retval)

print('---------------------------------')
retval = kmp_match('abfabababaecabaabab', 'ababae')
print(retval)

print('---------------------------------')
retval = kmp_match('aaaabaaaabaaaabaaaaa', 'aaaaa')
print(retval)

print('<================>')
s = 'abacabacabacabac'
retval = gen_next2(s)
print(s, len(s), retval, len(s) % (len(s) - retval[len(s) - 1] - 1))

print('<================>')
s = 'abaabaaba'
retval = gen_next2(s)
print(s, len(s), retval, len(s) % (len(s) - retval[len(s) - 1] - 1))

s = 'abaabaabaab'
retval = gen_next2(s)
expression='len(s) % (len(s) - retval[len(s) - 1] - 1)'
print('<================>')
print('expression', expression)
print(s, len(s), retval, eval(expression))


print(gen_next1("ababaabab"))