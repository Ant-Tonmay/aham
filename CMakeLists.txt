cmake_minimum_required(VERSION 3.16)

project(
    aham
    VERSION 1.0.0
    DESCRIPTION "Aham Programming Language"
    LANGUAGES CXX
)

# -----------------------------
# C++ standard
# -----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Core library
# -----------------------------
add_library(aham_core
    src/lexer/lexer.cpp
    src/lexer/utils/lexer_helpers.cpp
    src/parser/parser.cpp
    src/interpreter/interpreter.cpp
    src/interpreter/environment.cpp
    src/interpreter/expr_evaluator.cpp
    src/interpreter/stmt_executor.cpp
    src/interpreter/deep_copy.cpp
    src/symbol_table/symbol_table.cpp
    src/compiler/chunk.cpp
    src/compiler/compiler_core.cpp
    src/compiler/compiler_expr.cpp
    src/compiler/compiler_stmt.cpp
    src/vm/vm.cpp
    src/vm/vm_array.cpp
    src/vm/vm_call.cpp
    src/vm/vm_class.cpp
    src/vm/vm_ops.cpp
    src/vm/vm_trait.cpp
    src/vm/vm_builtins.cpp
    src/vm/vm_helper.cpp
    src/vm/utils/access_utils.cpp
    src/vm/utils/value_utils.cpp
    src/compiler/utils/serializer.cpp
    src/compiler/utils/deserializer.cpp
    src/loader/module_loader.cpp
    src/utils/utils.cpp
    src/dependency_graph/dependency_graph.cpp
    src/dependency_graph/dependency_scanner.cpp
    src/manifest/manifest.cpp
    src/manifest/manifest_loader.cpp
    src/manifest/manifest_parser.cpp

)

target_include_directories(aham_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# -----------------------------
# Compiler warnings
# -----------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(aham_core PRIVATE -Wall -Wextra -Wpedantic)
elseif (MSVC)
    target_compile_options(aham_core PRIVATE /W4)
endif()

# -----------------------------
# Main executable
# -----------------------------
add_executable(aham src/main.cpp)
target_link_libraries(aham PRIVATE aham_core)

# -----------------------------
# Tests
# -----------------------------
enable_testing()

install(TARGETS aham
    RUNTIME DESTINATION bin
)


set(CPACK_PACKAGE_NAME "Aham")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Aham Project")
set(CPACK_PACKAGE_CONTACT "team.aham@zohomail.in")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Aham Programming Language")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/Ant-Tonmay/aham")

# Debian
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tonmay")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "Aham")
set(CPACK_STRIP_FILES TRUE)

include(CPack)

# -----------------------------
# Debug flags
# -----------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(aham_core PRIVATE DEBUG)
endif()
