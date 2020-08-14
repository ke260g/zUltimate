# 更清晰地概览该路径的 文件状态
svn status . | sort
# del unversioned files
svn status . | grep -e "^?" | awk '{print $2}' | xargs rm -rf

# 重新维护工程时, 一定要 先revert再update, 如更新 母包
svn revert . --depth=infinity       --no-auth-cache
svn update . --set-depth=infinity   --no-auth-cache
# 设置链接
+ svn propedit svn:externals { directory }
# 保留log
svn rename
svn copy 
svn move # 这个不不会保留log

# TortoiseSVN 客户端默认 忽略.so, .exe 等二级制文件

# 层级 svn update revert 的逻辑
### 需求场景
# root/app/lib/moduleA 中
# 只想 revert & update moduleA, 
# 其他上级目录 不能revert -R
# 但是上级目录 可能不存在 需要update
### 实现逻辑
# 1. 如果 target目录存在, 则 revert && update
# 2. 否则 从最上级目录开始 逐级向下递归 一旦发现不存在则update

# Adavanced cleanup
sqlite3 .svn/wc.db "select * from work_queue"
sqlite3 .svn/wc.db "select * from wc_lock"
sqlite3 .svn/wc.db "delete from work_queue"
sqlite3 .svn/wc.db "delete from wc_lock"

# AS project svn propedit svn:ignore
*.iml
.idea/
.gradle/
.*build/
## gradle 手动 cleanup
find . -maxdepth 4 -type f -name "*.iml" -exec rm -rf {} \;
find . -maxdepth 4 -type d -name "build" -exec rm -rf {} \;
find . -maxdepth 4 -type d -name ".idea" -exec rm -rf {} \;
find . -maxdepth 4 -type d -name ".gradle" -exec rm -rf {} \;