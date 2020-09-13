def manacher(s):
    r = []
    for c in s:
        r.append('#')
        r.append(c)
    r.append('#')

    mn = [1 for i in range(len(r))]
    mx = 1
    iM = 0
    mn[0] = 1
    for i in range(1, len(r)):
        # 递推关系
        if mx > i:
            mn[i] = (min(mn[2*iM - i], mx - i))
        else:
            mn[i] = 1

        # 暴力求算
        while i + mn[i] < len(r) and r[i + mn[i]] == r[i - mn[i]]:
            mn[i] += 1

        if mx < i + mn[i]:
            mx = i + mn[i]
            iM = i
    print([c for c in s])
    print(r)
    print(mn, len(mn))
    print(mx)
    return mx, mn

mx, mn = manacher('abbabcba')
