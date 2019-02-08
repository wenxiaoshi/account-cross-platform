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
    name = "helloworld",
    srcs = ["protos/helloworld.proto"],
)

grpc_proto_library(
    name = "account",
    srcs = ["protos/account.proto"],
)

cc_binary(
    name = "account_server",
    srcs = ["cpp/account/account_server.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [":account", "//:grpc++",":hashmap"],
    data = ["//mypem:ca.crt","//mypem:server.crt","//mypem:server.key"],
    #copts = ["-Iinclude/mongo/include"],
)

cc_library(
    name = "hashmap",
    srcs = ["cpp/account/hash_map.cpp"],
    hdrs = ["cpp/account/hash_map.h"],
)

cc_binary(
    name = "greeter_client",
    srcs = ["cpp/helloworld/greeter_client.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [":helloworld", "//:grpc++"],
)

cc_binary(
    name = "greeter_server",
    srcs = ["cpp/account/greeter_server.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [":helloworld", "//:grpc++"],
)
