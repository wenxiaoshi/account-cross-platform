# 单终端登录系统


A Project Of How To Use gRPC/Bazel/Djinni/Docker/MySQL. 


## 目录

* [项目介绍](#项目介绍)
	* [功能简介](#功能简介)
	* [技术简介](#技术简介)
	* [架构图](#架构图)
	* [流程图](#流程图)
* [项目实现](#项目实现)
	* [接口说明](#接口说明)
	* [错误码说明](#错误码)
		* [公共错误码](#公共错误码)
		* [模块错误码](#模块错误码)
	* [加密说明](#加密说明)
		* [接口请求SSL](#接口请求SSL)
		* [密码初始化](#密码初始化)
		* [Token生成规则](#Token生成规则)
	* [日志说明](#日志说明)
		* [日志规则](#日志规则)
		* [日志样例](#日志样例)
    * [安全防范](#安全防范)
		* [密码强度](#密码强度)
		* [参数校验](#参数校验)
        * [敏感词过滤](#敏感词过滤)
        * [参数化查询](#参数化查询)
	* [数据库说明](#数据库说明)
	* [版本说明](#版本说明)
	* [常用后端命令](#常用后端命令)


## 项目介绍

### 功能简介

1. 具备注册登录功能
2. 一个⽤户只能在一个设备上登录，切换终端登录时，其他已登录的终端会被踢出
3. 用户长时间未使用功能，需要重新登录，而经常使用则会延长登录状态

 客户端下载链接：[MyLogin](https://fir.im/5vh8)

### 技术简介

- 客户端用Android实现，采用Djinni实现跨平台开发，业务逻辑在C++层实现
- 服务端用C++实现，缓存使用Redis,数据库使用MySQL
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
        <td><center></center></td>
        <td><center></center></td>
    </tr>
    <tr>
        <td><center><img src="https://melon-personal.oss-cn-shenzhen.aliyuncs.com/login-android.png"></center></td>
        <td><center><img src="https://melon-personal.oss-cn-shenzhen.aliyuncs.com/login-back.png"></center></td>
    </tr>
</table>

### 流程图

![](https://melon-personal.oss-cn-shenzhen.aliyuncs.com/wechat/pic_project_flow.png)

## 项目实现

### 接口说明
| 接口名  | 接口方法 |
|:------|------|
| [用户登录](#用户登录) | requestUserLogin |
| [用户注册](#用户注册) | requestUserSign |
| [退出登录](#退出登录) | requestLogout |
| [检查用户在线状态](#检查用户在线状态) | checkConnect |
| [刷新用户Token](#刷新用户Token) | refreshToken |

#### 用户登录
##### 请求方法

requestUserLogin

##### 请求参数

| 参数名  | 必选 | 类型 | 说明 |
|:------|------|------|------|
| account | true | string | 用户账号 |
| password | true | string | 用户密码 |

##### 返回参数

| 参数名  | 类型 | 说明 |
|:------|------|------|
| token | string | 用户Token |
| refresh_token | string | 用来刷新TOken |

##### 返回结果
```
{
    "code": 0,
    "msg": "",
    "data": {
	"token":"",
	"refresh_token":""
    }
}
```

#### 用户注册
##### 请求方法

requestUserSign

##### 请求参数

| 参数名  | 必选 | 类型 | 说明 |
|:------|------|------|------|
| account | true | string | 用户账号 |
| password | true | string | 用户密码 |

##### 返回参数

| 参数名  | 类型 | 说明 |
|:------|------|------|
| token | string | 用户Token |
| refresh_token | string | 用来刷新TOken |

##### 返回结果
```
{
    "code": 0,
    "msg": "",
    "data": {
	"token":"",
	"refresh_token":""
    }
}
```

#### 退出登录
##### 请求方法

requestLogout

##### 请求参数

| 参数名  | 必选 | 类型 | 说明 |
|:------|------|------|------|
| token | true | string | 用户Token |

##### 返回参数

无

##### 返回结果
```
{
    "code": 0,
    "msg": "",
    "data": ""
}
```

#### 检查用户在线状态
##### 请求方法

checkConnect

##### 请求参数

| 参数名  | 必选 | 类型 | 说明 |
|:------|------|------|------|
| token | true | string | 用户Token |

##### 返回参数

无

##### 返回结果
```
{
    "code": 0,
    "msg": "",
    "data": ""
}
```

#### 刷新用户Token
##### 请求方法

refreshToken

##### 请求参数

| 参数名  | 必选 | 类型 | 说明 |
|:------|------|------|------|
| token | true | string | 用户Token |
| refresh_token | true | string | 用来刷新TOken |

##### 返回参数

无

##### 返回结果
```
{
    "code": 0,
    "msg": "",
    "data": {
	"token":"",
	"refresh_token":""
    }
}
```

### 错误码

#### 公共错误码

公共错误码范围 0 ~ 99

| 错误代码  | 错误类型  | 描述 
|:------|------|------|
| 0 | 请求成功 |  |
| 1 | 请求参数非法 | 后台接口接收到的参数不合法，如账号里包含非法字符，密码不够位数 |
| 2 | 返回数据非法 | 返回的数据中存在敏感字符 |

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
| 1001004 | 用户Token更新失败 | 根据账号与UID生成的Token，更新到Redis失败 |
| 1002xxx | | | 用户注册接口相关 |
| 1002001 | 账号已经存在 | 根据接口传来的账号，查询数据库发现该账号已经存在 |
| 1002002 | 密码初始化失败 | 对接口传来的密码，进行加密过程失败 |
| 1002003 | 账号创建失败 | 根据账号信息新增账号到数据库失败 |
| 1002004 | 账号信息获取失败 | 根据账号查询数据库对应账号信息失败 |
| 1002005 | 账号会话信息创建失败 | 根据账号与UID生成的Token，更新到Redis失败 |
| 1003xxx | | | 用户退出登录接口相关 |
| 1003001 | Token格式错误 | 接口传来的Token格式不正确，属于非法Token |
| 1003002 | 账号会话信息更新失败 | 更新Token信息到Redis失败 |
| 1003003 | 账号Token已失效 | 用户Token失效（过期） |
| 1004xxx | | | 检查在线状态接口 |
| 1004001 | Token格式错误 | 接口传来的Token格式不正确，属于非法Token |
| 1004002 | Token已经失效 | 用户Token失效（过期），需要客户端执行刷新Token操作 |
| 1004003 | Token校验失败 | 根据Token与数据库中Token进行比对，判断该设备不在线 |
| 1005xxx | | | 刷新用户Token接口 |
| 1005001 | Token格式错误 | 接口传来的Token格式不正确，属于非法Token |
| 1005002 | RefreshToken格式错误 | 接口传来的RefreshToken格式不正确 |
| 1005003 | RefreshToken不对应 | 根据UID从Redis取出RefreshToken进行比对不相等 |
| 1005004 | RefreshToken已失效 | 用户RefreshToken失效（过期），需要重新登录 |
| 1005005 | 账号会话信息创建失败 | 根据账号与UID生成的Token，更新到Redis失败 |

### 加密说明

#### 接口请求SSL

接口使用SSL加密认证，通过OpenSSL生成对应的密钥文件，客户端保存公钥，在Channel中加上公钥信息，服务器端通过私钥认证接口合法性，保证传输安全

#### 密码初始化

<img src="http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login_pwd.png" width = 442 height = 410 />

##### 用户注册流程，后台接收到用户的账号和密码

1. 通过SHA256消息摘要算法对账号进行计算，获得摘要信息
2. 将1中获得的摘要信息作为盐，与密码拼接，获得加密前信息
3. 将2中获得的信息，进行MD5消息摘要计算，获得加密后的128位，用16进制表示的密码字符串

##### 用户登录流程，后台接收到用户的账号和密码

1. 通过用户传来的账号和密码，用“注册流程”加密规则获得加密后密码字符串
2. 与数据库中用户的密码字符串比对，一致则登录成功，反之则登录失败。

#### Token生成规则

<img src="http://melon-personal.oss-cn-shenzhen.aliyuncs.com/login_token.png" width = 747 height = 205 />

##### 用户登录/注册流程，生成用户Token并返回

1. 加密前Token由用户信息和当前登录信息组成

	加密前Token = [用户账号]:[用户UID]:[随机6位字符串]:[登录时间戳（秒）]:[过期时间戳（秒）]

	例如："13533192331:2:172649:1552031125:1557301525"

2. 将加密前Token进行对称密钥算法AES计算，获得加密后Token

	Token = AES(加密前Token)

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

### 安全防范

#### 密码强度

分三个等级

1. 初级密码
 * 总共6-18位，
 * 由任意数字或字母组成

2. 中级密码
 * 总共6-18位，
 * 至少1个数字字符
 * 至少1个小写字母
 * 至少1个大写字母
 * 其他可以是任意字符

3. 高级密码
 * 总共6-18位，
 * 至少1个数字字符
 * 至少1个小写字母
 * 至少1个大写字母
 * 至少1个特殊字符
 * 其他可以是任意字符

#### 参数校验

1. 入参校验
    
    根据参数的业务类型规定数据的格式，进行校验，不匹配则返回错误码1，防止SQL注入。

2. 反参校验

    返回数据前对数据进行敏感字符过滤，存在则返回错误码2，防止XSS攻击返回非法语句，SSRF攻击返回系统信息等。

#### 敏感词过滤

在执行SQL语句前，对传入的参数进行敏感字符过滤，确保参数内容符合预期，防止SQL注入。

#### 参数化查询

使用存储过程进行参数化查询和数据的插入，防止SQL注入。


### 数据库说明

使用MySQL作为数据库

| 表名 | 字段  | 类型  | 备注 |
|:--|----|------|------|
| user_account |  |  ||
|  | ID | INTEGER PRIMARY KEY | 用户UID |
|  | ACCOUNT | CHAR | 用户账号 |
|  | PASSWORD | CHAR | 用户密码 |

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
| 系统 |  |  ||
|| ufw | 0.35 | Ubuntu 防火墙 |

### 常用后端命令
| 命令 | 备注 |
|:--|----|
| bazel build //source:account_server | 构建目标文件 |
| cp ./bazel-bin/source/account_server docker-src/ | 将构建成功的执行文件复制到本地docker目录 |
| docker build -t grpcserver:1.0 . | 在根目录，根据Dockerfile编写的规则，生成服务镜像 |
| docker-compose up grpcserver | 在根目录，根据docker-compose.yml编写的规则，启动并管理容器 |
