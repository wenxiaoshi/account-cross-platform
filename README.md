# 单终端登录系统

## 目录

* [项目介绍](#项目介绍)
	* [功能简介](#功能简介)
	* [技术简介](#技术简介)
	* [架构图](#架构图)
	* [流程图](#流程图)
* [项目实现](#项目实现)
	* [接口说明](#接口说明)
	* [错误码说明](#错误码说明)
	* [日志说明](#日志说明)
		* [日志规则](#日志规则)
		* [日志样例](#日志样例)
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
| 接口名  | 含义  | 备注 
|:------|------|------|
| requestUserLogin | 请求登录接口 |  |
| requestUserSign | 请求注册接口 |  |
| requestLogout | 退出登录接口 |  |
| checkConnect | 检查在线状态接口 |  |

### 错误码说明

| 错误码  | 含义  | 备注 
|:------|------|------|
| 0 | 成功 |  |
| -1 | 失败 | 默认 |
| 1010 | pem文件不存在 |  |
| 2000 | 该账号不存在 |  |
| 2001 | 用户密码错误 |  |
| 2002 | 该账号已注册 |  |
| 2003 | 更新账号的token失败 |  |
| 2004 | 新增账号失败 |  |
| 2005 | 该账号在另一端登录 |  |
| 2006 | 用户token不合法 |  |
| 2007 | 初始化密码失败 |  |
| 2008 | 用户token已过期 |  |
| 2009 | 该账号不在线 |  |
| 2010 | 新增账号Session失败 |  |
| 2011 | 接口参数错误 |  |

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
