# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Find "ModuleTools" dependencies, which are other ModuleTools packages.
set(Qt6GrpcTools_FOUND TRUE)

set(__qt_GrpcTools_tool_third_party_deps "WrapProtoc\\;FALSE\\;\\;\\;")
_qt_internal_find_third_party_dependencies("GrpcTools" __qt_GrpcTools_tool_third_party_deps)

set(__qt_GrpcTools_tool_deps "Qt6ProtobufTools\;6.9.0")
_qt_internal_find_tool_dependencies("GrpcTools" __qt_GrpcTools_tool_deps)
