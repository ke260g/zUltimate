# collect message from the remote server
check_parameters $*       # 检查用户输入参数
show_parameters           # 显示用户输入参数
check_network             # 检查网络环境
build_environment         # 构建对端基础环境
exec_task_scripts         # 遍历日志收集脚本
upload_report             # 上传报告日志