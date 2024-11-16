## src下cmake分析

---

```cmake
set(header_path ${${PROJECT_NAME}_SOURCE_DIR}/include/mytest)
set(detail_header
    ${header_path}/detail/add.h
)
set(header
    ${header_path}/mytest.h
)
set(src
    mytest.cc
    detail/add.cc
)
```

这里先把头文件路径定义好，方便后面的使用
然后就是定义头文件和源文件的具体变量

---

```cmake
# configure config file
configure_file("config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp")
```

这里是用来生成配置文件，首先有一个config.hpp.in也在当前目录下

```cpp
// Copyright (C) 2022-2024 gxt_kt contributors

#ifndef MY_LIBRARY_IMPL_IN_CONFIG_HPP
#error "do not include this file directly, use config.hpp"
#endif

#include <cstddef>

//=== options ===//
// clang-format off
#cmakedefine01 MY_LIBRARY_IMPL_TEST1
#define MY_LIBRARY_IMPL_TEST2 ${MY_LIBRARY_IMPL_TEST2}
// clang-format on
```

这里主要是两个地方会改：
- #cmakedefine01 MY_LIBRARY_IMPL_TEST1
#cmakedefine01会把后面的宏定义改成对应cmake变量的值
- #define MY_LIBRARY_IMPL_TEST2 ${MY_LIBRARY_IMPL_TEST2}

这里会把 ${MY_LIBRARY_IMPL_TEST2}变量替换成cmake变量的值
然后`configure_file`会把这个文件变成config_impl.hpp文件

---

```cmake
add_library(${MY_LIB_NAME} SHARED ${detail_header} ${header} ${src})
target_include_directories(${MY_LIB_NAME}
                             PUBLIC
                                $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include/mytest>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for generated files in build mode
                                $<INSTALL_INTERFACE:${MY_LIBRARY_INC_INSTALL_DIR}/mytest> # for client in install mode
                             PRIVATE
                                ${header_path} # for source files
)
target_compile_definitions(${MY_LIB_NAME} PUBLIC
                            MY_LIBRARY_VERSION=${MY_LIBRARY_VERSION}
                            MY_LIBRARY_VERSION_MAJOR=${MY_LIBRARY_VERSION_MAJOR}
                            MY_LIBRARY_VERSION_MINOR=${MY_LIBRARY_VERSION_MINOR}
                            MY_LIBRARY_VERSION_PATCH=${MY_LIBRARY_VERSION_PATCH}
)
# set add_library lib name
set_target_properties(${MY_LIB_NAME} PROPERTIES
                                       OUTPUT_NAME "${MY_LIB_NAME}.${MY_LIBRARY_VERSION}"
                                       POSITION_INDEPENDENT_CODE ON)
```

这里就是实际添加自己的库并设置库的属性

`add_library`就是添加库，**SHARED** 用来设置成动态库 **.so**

`target_include_directories`设置的基本就是套路

`target_compile_definitions`就是在编译库的时候给定宏定义，这样编译时一些宏定义会被替换进来

`set_target_properties`就是设置生成的库的名字，这里给名字加上了版本的后缀

---

```cmake
# if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
#     if("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
#         target_compile_options(foonathan_memory PRIVATE /WX /W3 /D _CRT_SECURE_NO_WARNINGS)
#     else()
#         target_compile_options(foonathan_memory PRIVATE -pedantic-errors -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
#     endif()
# elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
#     target_compile_options(foonathan_memory PRIVATE -pedantic-errors -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
# elseif(MSVC)
#     target_compile_options(foonathan_memory PRIVATE /WX /W3 /D _CRT_SECURE_NO_WARNINGS)
# endif()
```

这里就是针对不同的编译器的时候，设置不同的编译属性

不过这里注释掉了，没用上

---

```cmake
# install lib
install(TARGETS ${MY_LIB_NAME} EXPORT ${MY_LIB_NAME}_targets
        RUNTIME       DESTINATION ${MY_LIBRARY_RUNTIME_INSTALL_DIR}
        LIBRARY       DESTINATION ${MY_LIBRARY_LIBRARY_INSTALL_DIR}
        ARCHIVE       DESTINATION ${MY_LIBRARY_ARCHIVE_INSTALL_DIR}
        FRAMEWORK     DESTINATION ${MY_LIBRARY_FRAMEWORK_INSTALL_DIR}
)
```

这里就是实际配置文件的安装目录了

会把配置都写到目标${MY_LIB_NAME}_targets中，根目录的CMakeLists.txt会依照这个生成cmake的配置文件

分别有可执行文件，动态库静态库，FRAMEWORK用来macos上的框架位置

---

```cmake
# Write/install version file
include(CMakePackageConfigHelpers) # import cmake version file generator
# set version_file name
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/${MY_LIB_NAME}-config-version.cmake")
# generate version_file
write_basic_package_version_file(${version_file}
                                 VERSION ${MY_LIBRARY_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
```

这里用来配置库的版本信息，用到cmake的内置函数`write_basic_package_version_file`

所以先导入cmake的内置函数库，然后设置要生成的文件位置，然后生成文件

**COMPATIBILITY** 兼容性这里也会影响到cmake使用`find_package`查找库时设置的版本

---

```cmake
# install files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME}/)
install(FILES ${header}                                   DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME})
install(FILES ${detail_header}                            DESTINATION ${MY_LIBRARY_INC_INSTALL_DIR}/${MY_LIB_NAME}/detail)
install(FILES ${version_file}                             DESTINATION ${MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR})
```

这里就是实际上进行安装的了，会把不同的文件安装到不同的位置

---
