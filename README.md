# 单终端登录系统

## 目录

* [项目介绍](#项目介绍)
	* [功能简介](#功能简介)
	* [技术简介](#技术简介)
	* [架构图](#架构图)
	* [流程图](#流程图)
* [项目实现](#项目实现)
	* [接口说明](#接口说明)
	* [错误码](#错误码)
		* [公共错误码](#公共错误码)
		* [模块错误码](#模块错误码)
	* [日志说明](#日志说明)
	* [数据库说明](#数据库说明)
	* [版本说明](#版本说明)
	* [常用后端命令](#常用后端命令)
* [更新日志](#更新日志)
	* [2019-3-4](#2019-3-4)
	* [2019-2-10](#2019-2-10)


## 项目介绍

### 功能简介

1. 具备注册登录功能
2. 一个⽤户只能在一个设备上登录，切换终端登录时，其他已登录的终端会被踢出

 客户端下载链接：[MyLogin](https://fir.im/5vh8)

### 技术简介

- 客户端用Android实现，采用Djinni实现跨平台开发，业务逻辑在C层实现
- 服务端用C++实现，数据库使用SQLite3
- 使用GPRC进行远程通信，生成非对称密钥，使用SSL进行数据传输加密
- 使用Bazel构建后台项目，以及依赖管理
- 使用Docker部署项目，并使用Docker Compose编排容器

### 架构图

<table>
    <tr>
        <td><center>客户端</center></td>
        <td><center>服务端</center></td>
    </tr>
    <tr>
        <td><center><img src="http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login-android.png"></center></td>
        <td><center><img src="http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login-android.png"></center></td>
    </tr>
</table>

### 流程图

![](http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login-design.png)

## 项目实现

### 接口说明
| 接口名  | 方法 | 参数 | 返参 |
|:------|------|------|------|
| 用户登录 | requestUserLogin | account password | token |
| 用户注册 | requestUserSign | account password | token |
| 退出登录 | requestLogout | token | |
| 检查用户在线状态 | checkConnect | token | |

### 错误码

#### 公共错误码

公共错误码范围 0 ~ 99

| 错误代码  | 错误类型  | 描述 
|:------|------|------|
| 0 | 请求成功 |  |
| 1 | 请求参数非法 | 后台接口接收到的参数不合法，如账号里包含字符，密码不够位数 |

#### 模块错误码

业务逻辑错误，7位数字

组成规则：[模块][接口][具体错误] 

如：1xxxzzz 账号模块，2xxxzzz 卡包模块, ......

| 错误代码  | 错误类型  | 描述 | 备注 |
|:------|------|------|----|
| 1001xxx | | | 用户登录接口相关 |
| 1001001 | 账号不存在 | 根据接口传来的账号，查询数据库没有找到对应的账号信息 |
| 1001002 | 密码初始化失败 | 对接口传来的密码，进行加密过程失败 |
| 1001003 | 用户密码错误 | 对接口传来的密码，与数据库中该用户的密码匹对失败 |
| 1001004 | 用户Token更新失败 | 通过账号与UID生成的Token，更新到数据库失败 |
| 1002xxx | | | 用户注册接口相关 |
| 1002001 | 账号已经存在 | 根据接口传来的账号，查询数据库发现该账号已经存在 |
| 1002002 | 密码初始化失败 | 对接口传来的密码，进行加密过程失败 |
| 1002003 | 账号创建失败 | 根据账号信息新增账号到数据库失败 |
| 1002004 | 账号信息获取失败 | 根据账号查询数据库对应账号信息失败 |
| 1002005 | 账号会话信息创建失败 | 根据账号信息新增账号会话（Token）到数据库失败 |
| 1003xxx | | | 用户退出登录接口相关 |
| 1003001 | Token格式错误 | 接口传来的Token格式不正确，属于非法Token |
| 1003002 | 账号会话信息更新失败 | 更新Token信息到会话表失败 |
| 1004xxx | | | 检查在线状态接口 |
| 1004001 | Token格式错误 | 接口传来的Token格式不正确，属于非法Token |
| 1004002 | Token已经失效 | 根据Token中附带的过期时间，判断该Token已经失效 |
| 1004003 | 账号已离线 | 根据Token查询会话信息，判断该账号是离线状态 |
| 1004004 | Token校验失败 | 根据Token与数据库中Token进行比对，判断该设备不在线 |

### 日志说明

#### 日志规则

日志文件按日期分割，例如 2019-3-5.log

| 消息类型  | 含义  |
|:------|------|
| INFO | 相关INFO信息可以让运维保留下来，可用来分析，比如接口访问信息 |
| DEBUG | 相关DEBUG信息提供给开发来用调试程序 |
| WARN | 相关WARN信息提示程序没有按照预期的运行，但不会影响到整体正常运行，需要被FIX |
| ERROR | 相关ERROR信息提示程序出现了严重错误，影响到系统的正常运行，必须被FIX |

#### 日志样例

![](http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login_log.png)

### 数据库说明

使用SQLite3作为数据库

| 表名 | 字段  | 类型  | 备注 |
|:--|----|------|------|
| user_account |  |  ||
|  | ID | INTEGER PRIMARY KEY | 用户UID |
|  | ACCOUNT | CHAR | 用户账号 |
|  | PASSWORD | CHAR | 用户密码 |
| user_session |  |  ||
|  | ID | INTEGER PRIMARY KEY | |
|  | UID | INTEGER | 用户UID，外键关联 user_account（ID）|
|  | TOKEN | CHAR | 用户Token |
|  | IS_ONLINE | INTEGER | 是否在线（1.在线 0.离线） |

### 版本说明

|| 模块  | 版本号  | 备注 |
|:--|----|------|------|
| Android端 |  |  ||
|| Android Studio | 3.2.1 | Android IDE |
|| Gradle | 4.6 |  |
|| Android Plugin Version | 3.2.1 |  |
|| GRPC-C++ | 1.18.0 |  |
|| Protobuffer | 3.6.1 |  |
| 后端 |  |  ||
|| Ubuntu | 18.04-x86_64 |  |
|| Bazel | 0.20.0 |  |
|| GRPC-C++ | 1.18.0 |  |
|| Docker | 18.09.1 | Community |
|| Docker Compose | 1.23.2 |  |

### 常用后端命令
| 命令 | 备注 |
|:--|----|
| bazel build //source:account_server | 构建目标文件 |
| cp ./bazel-bin/source/account_server docker-src/ | 将构建成功的执行文件复制到本地docker目录 |
| docker build -t grpcserver:1.0 . | 在根目录，根据Dockerfile编写的规则，生成服务镜像 |
| docker-compose up grpcserver | 在根目录，根据docker-compose.yml编写的规则，启动并管理容器 |


## 更新日志
### 2019-3-4

##### 客户端

1. 补充参数校验
2. 将网络实现改为 GRPC-C++
3. 将数据存储改为C++实现（key-value，采用读写文件的方式）

##### 服务器端

1. 补充参数校验
2. DB采用Sqlite进行数据存储
3. 用户密码初始化，加密处理

	3-1. 将用户账号进行Sha256消息摘要计算
	
		salt = Sha256 (account)
		
	3-2. 将salt与密码合并，获得字符数组，进行MD5计算
	
	    encrypt_password = MD5 (salt + password)
4. 登录后返回Token

	4-1. 获得加密前Token
	
		Info = [UID]:[Account]:[随机6位数字]:[生成Token时间（秒）]:[Token过期时间（秒）]
		
	4-2. 用AES对称算法进行加密处理
	
		Toekn = AES ("Info"）
5. 根据Token判断登录状态是否过期
6. 根据Token判断用户是否在线
7. 补充代码日志和接口日志，信息保存到文件中（文件按日期分）

### 2019-2-10

提交可运行版本1.0，详情可看TAG-1.0.0的README.md
