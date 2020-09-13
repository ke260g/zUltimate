import pprint

def str2ary(s):
    return [ c for c in s ]

def ary2str(a):
    return ''.join(a)

def LCS(X, Y):
    # init
    mX, mY = [' '], [' ']
    mX.extend(X)
    mY.extend(Y)
    lX, lY = len(mX), len(mY)
    dp = [ [{'status':None, 'length':0}] * lY for i in range(lX) ]

    # step_0
    # build 2D map
    for i in range(1, lX):
        for j in range(1, lY):
            if mX[i] == mY[j]:
                dp[i][j] = {
                        'status': 'LeftTop',
                        'length': dp[i-1][j-1]['length'] + 1
                        }
            elif dp[i-1][j]['length'] > dp[i][j-1]['length']:
                dp[i][j] = {
                        'status': 'Top',
                        'length': dp[i-1][j]['length']
                        }

            elif dp[i-1][j]['length'] < dp[i][j-1]['length']:
                dp[i][j] = {
                        'status': 'Left',
                        'length': dp[i][j-1]['length']
                        }
            else: # dp[i - 1][j]['length'] == dp[i][j - 1]['length']:
                dp[i][j] = {
                        'status': 'TopOrLeft',
                        'length': dp[i][j-1]['length']
                        }

    full = [ [(lX - 1, lY - 1)] ]
    idx = 0
    while idx < len(full):
        (i, j) = full[idx][0]
        if i == 1 or j == 1:
            idx += 1 # 到达边界; 终止
            continue
        if dp[i][j]['status'] == 'LeftTop': # 头插法
            full[idx].insert( 0, (i-1, j-1) )
        elif dp[i][j]['status'] == 'Left':  # 头插法
            full[idx][0] = (i, j-1)
        elif dp[i][j]['status'] == 'Top':   # 头插法
            full[idx][0] = (i-1, j)
        else: # 'TopOrLeft'       # 路径分裂; 新的解
            copy = full[idx].copy()
            full[idx][0] = (i, j-1) # 上边
            copy[0] = (i-1, j) # 左边
            full.append( copy )

    # 回溯 step_3
    # 从多个解中抽取 LCS
    res = [ [] for i in range(len(full)) ]
    pprint.pprint(res)
    for i  in range(len(full)):
        for (x, y) in full[i]:
            res[i].append(mX[x])
    #############
    pprint.pprint(res)

    return dp[lX-1][lY-1]['length'], res

x = 'abcbdab'
y = 'bdcaba'
length, res = LCS(str2ary(x), str2ary(y))
print('-------------')
print(x, y)
pprint.pprint(length)
pprint.pprint(res)
