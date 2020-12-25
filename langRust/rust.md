```sh
apt install rustc cargo

cargo new $proj_name # 创建项目

cargo run            # 调试模式, 没有优化选项
cargo run --release  # 发布模式, 加上优化选项
cargo clean          # 删除编译生成的文件
```

# 变量定义 (let/mut/const)
4中状态: unbind, let, let mut, const
```c++
  /---> (  let  ) --->\
 /         |   ^       \
{          |   |        }->(const)
 \         v   |       /
  \---> (let mut) --->/
```
1. 变量可以重新绑定, (即重复 let)
2. let     类型的变量, 是只读的
3. let mut 类型的变量, 是可读写的
4. 常亮不能重新绑定, 也不能重设为其他常量