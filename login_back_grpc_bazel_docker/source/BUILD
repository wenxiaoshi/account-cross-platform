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
    deps = [":account", "//:grpc++",":hashmap",":commonutils",":sqlite3",":my_log",":my_db",":my_model",":my_constant"],
    data = ["//mypem:ca.crt","//mypem:server.crt","//mypem:server.key"],
)

cc_library(
    name = "hashmap",
    srcs = ["cpp/account/model/hash_map.cpp"],
    hdrs = ["cpp/account/model/hash_map.h"],
)

cc_library(
    name = "commonutils",
    srcs = ["cpp/account/utils/common_utils.cpp"],
    hdrs = ["cpp/account/utils/common_utils.h"],
    deps = [":cipher",":my_constant",":my_log"],
)

cc_library(
    name = "cipher",
    srcs = ["cpp/account/libs/aes_encryptor.cpp","cpp/account/libs/aes.cpp","cpp/account/libs/md5.cc","cpp/account/libs/sha256.cc"],
    hdrs = ["cpp/account/libs/aes_encryptor.h","cpp/account/libs/aes.h","cpp/account/libs/md5.h","cpp/account/libs/sha256.h"],
)

cc_library(
    name = "my_log",
    srcs = ["cpp/account/log/my_log.cc"],
    hdrs = ["cpp/account/log/my_log.h"],
    deps = [":file_utils"],
)

cc_library(
    name = "my_db",
    srcs = ["db/my_db.cc"],
    hdrs = ["db/my_db.h"],
    deps = [":sqlite3",":my_model",":my_log"],
)

cc_library(
    name = "sqlite3",
    srcs = ["sqlite3/sqlite3.c","sqlite3/shell.c"],
    hdrs = ["sqlite3/sqlite3.h","sqlite3/sqlite3ext.h"],
    linkopts = ["-ldl"],
)

cc_library(
    name = "my_model",
    srcs = ["cpp/account/model/user_account.cc","cpp/account/model/user_session.cc"],
    hdrs = ["cpp/account/model/user_account.h","cpp/account/model/user_session.h"],
)

cc_library(
    name = "my_constant",
    srcs = ["cpp/account/constant/my_constant.cc"],
    hdrs = ["cpp/account/constant/my_constant.h"],
)

cc_library(
    name = "file_utils",
    srcs = ["cpp/account/utils/file_utils.cpp"],
    hdrs = ["cpp/account/utils/file_utils.h"],
)