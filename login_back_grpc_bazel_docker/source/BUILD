# Copyright 2017 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])  # 3-clause BSD

package(default_visibility = ["//visibility:public"])

load("//bazel:grpc_build_system.bzl", "grpc_proto_library")

grpc_proto_library(
    name = "account",
    srcs = ["protos/account.proto"],
)

cc_binary(
    name = "account_server",
    srcs = ["cpp/account/account_server.cc"],
    deps = [":account", "//:grpc++",":my_model",":my_constant",":common_utils",":log_utils",":param_utils",":db_manager",":redis_manager",":conf"],
    data = ["//source/pem:ca.crt","//source/pem:server.crt","//source/pem:server.key"],
    linkopts = ["-lmysqlclient"],
)

cc_library(
    name = "my_model",
    srcs = ["cpp/account/model/user_account.cc","cpp/account/model/user_session.cc"],
    hdrs = ["cpp/account/model/user_account.h","cpp/account/model/user_session.h"],
)

cc_library(
    name = "my_constant",
    srcs = ["cpp/constant/my_constant.cc"],
    hdrs = ["cpp/constant/my_constant.h"],
)

#cc_library(
#    name = "hashmap",
#    srcs = ["cpp/struct/hash_map.cpp"],
#    hdrs = ["cpp/struct/hash_map.h"],
#)

# project utils

cc_library(
    name = "common_utils",
    srcs = ["cpp/utils/common_utils.cpp"],
    hdrs = ["cpp/utils/common_utils.h"],
    deps = [":cipher",":my_constant",":log_utils"],
)

cc_library(
    name = "log_utils",
    srcs = ["cpp/utils/log_utils.cc"],
    hdrs = ["cpp/utils/log_utils.h"],
    deps = [":file_utils"],
)

cc_library(
    name = "file_utils",
    srcs = ["cpp/utils/file_utils.cpp"],
    hdrs = ["cpp/utils/file_utils.h"],
)

cc_library(
    name = "param_utils",
    srcs = ["cpp/utils/param_utils.cpp"],
    hdrs = ["cpp/utils/param_utils.h"],
    deps = [":common_utils"],
)

# project conf
cc_library(
    name = "conf",
    srcs = ["cpp/manager/conf/server_conf.cpp"],
    hdrs = ["cpp/manager/conf/server_conf.h"],
    deps = [":json",":log_utils"],
    data = ["//source/conf:server_conf.json"],
)

# project redis
cc_library(
    name = "redis_manager",
    srcs = ["cpp/manager/redis/redis_manager.cpp"],
    hdrs = ["cpp/manager/redis/redis_manager.h"],
    deps = [":conf",":hiredis",":log_utils"],
)

# project db
cc_library(
    name = "db_manager",
    srcs = ["cpp/manager/db/db_manager.cpp"],
    hdrs = ["cpp/manager/db/db_manager.h"],
    deps = [":conf",":my_model",":log_utils",":db_base"],
)

cc_library(
    name = "db_base",
    srcs = ["cpp/manager/db/db_base.cpp"],
    hdrs = ["cpp/manager/db/db_base.h"],
    linkopts = ["-lmysqlclient"],
    deps = [":log_utils"],
)


# project deps

cc_library(
    name = "cipher",
    srcs = ["libs/cipher/aes_encryptor.cpp","libs/cipher/aes.cpp","libs/cipher/md5.cc","libs/cipher/sha256.cc"],
    hdrs = ["libs/cipher/aes_encryptor.h","libs/cipher/aes.h","libs/cipher/md5.h","libs/cipher/sha256.h"],
)

cc_library(
    name = "json",
    srcs = ["libs/json/json_reader.cpp","libs/json/json_value.cpp","libs/json/json_writer.cpp"],
    # hdrs = ["libs/json/autolink.h","libs/json/config.h","libs/json/features.h","libs/json/forwards.h","libs/json/json.h","libs/json/reader.h","libs/json/value.h","libs/json/writer.h","libs/json/json_batchallocator.h"],
    hdrs = glob(["libs/json/*.h","libs/json/*.inl"]),
)

cc_import(
    name = "hiredis",
    hdrs = ["//source/libs/redis:hiredis.h"],
    static_library = "//source/libs/redis:libhiredis.a"
)

