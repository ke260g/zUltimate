[TOC]
业务中常用到的图
# 静态结构图 (算是简化版的UML)
描述模块间的"关系",
简单的关系类型: 无关系, 单向数据通讯, 双向数据通讯, 依赖, 协同 (尽量尽量不要业务上去)

# 通讯流程图
```ditaa
A         B         C
|         |         |
+ -- 1 -> +         +
|         + -- 2 -> +
|         |         o
|         + <- 3 -- +
+ <- 3 -- +
```

# 程序逻辑图
事件, 判断, 循环

# Solution
静态结构图要列全, 上面分发人手
完整的通讯流程图是必要的
状态图: 严格的状态图是 开始是进程启动, 结束是进程死亡, 然后响应所有"抽象事件"
  这时就需判定"抽象事件"有没有列全
拆分通讯流程图: 通讯流程中的子 scanario 细化中间各个过程

## 通用故障场景
+ 设备坏了 (哪些设备坏了, 2进制穷举所有场景)
+ 设备升级 (哪些设备升级, 2进制穷举所有场景)
+ 设备更换 (哪些设备升级, 2进制穷举所有场景)
+ 进程重启(状态机的完备性)
+ 通讯过程异常
## 通用连接场景
+ 准备
+ 建立
+ 通讯(通讯本体场景: 进行哪些数据的通讯, 收的类型, 发的类型)
+ 关闭
+ 重连(连接本体掉线, 连接参数更新, 通讯逻辑故障, 通讯逻辑必然)
## 通讯本体场景


# 代码模型
## solution1
类   := 作为最底层的"相对原子"模块
主流程:= 初始化/销毁 一系列子模块
子模块:= 对象管理器的 集合与抽象
五态方法: 负责的业务的抽象, 写BUG的舞台,
         一般多传入 类对象, 然后回传结果, 交由上层管理
### 无态方法
### 对象管理器
### 类
### 主流程