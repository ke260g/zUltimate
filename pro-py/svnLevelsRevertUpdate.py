from functools import reduce
import os
def doShellCommand(command):
    print(command)

# 需求场景
# root/app/lib/moduleA 中
# 只想 revert & update moduleA,
# 其他上级目录 不能revert -R
# 但是上级目录 可能不存在 需要update

# 实现逻辑
# 1. 如果 target目录存在, 则 revert && update
# 2. 否则 从最上级目录开始, 逐级往下查, 一旦发现不存在则update
def svnLevelsRevertUpdate(dirs):
    if not isinstance(dirs, list):
        return

    # 先判断 target_dir
    target_dir = '/'.join(dirs)
    if os.path.exists(target_dir):
        doShellCommand('svn re -R %s'%(target_dir))
        doShellCommand('svn up %s'%(target_dir))
        return

    # 后逐级判断
    prev = ''
    for curr in dirs:
        prev = os.path.join(prev, curr)
        if not os.path.exists(prev):
            doShellCommand('svn up %s --set-depth=infinity'%(prev))
            break

# 测试
svnLevelsRevertUpdate(['root', 'app', 'lib', 'moduleA'])
