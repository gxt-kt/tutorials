# 首先是根目录下的CMakeLists.txt

我们按照顺序来分析

---

```cmake
# set(MY_LIB_NAME ${PROJECT_NAME})
set(MY_LIB_NAME "mylib")
message("[info]: set lib name: ${MY_LIB_NAME}")
```

这里是设置库的名字，后面的库名和安装路径都是以这个为准

---

```cmake
include(cmake/help.cmake)
```
导入一些cmake函数

---

```cmake
# define library version (update: apparently you can also do it in project()!)
set(MY_LIBRARY_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(MY_LIBRARY_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(MY_LIBRARY_VERSION_PATCH 2 CACHE STRING "patch version" FORCE)
set(MY_LIBRARY_VERSION ${MY_LIBRARY_VERSION_MAJOR}.${MY_LIBRARY_VERSION_MINOR}.${MY_LIBRARY_VERSION_PATCH} CACHE STRING "version" FORCE)
PrintVariable(MY_LIBRARY_VERSION)
```
上来的几个set(MY_LIBRARY_VERSION)啥的都是用来设置库的版本，这给后面的一些生成库版本后缀提供了支持

---

```cmake
set(CMAKE_BUILD_TYPE Release) # Debug Release RelWithDebInfo
# set a debug postfix
set(CMAKE_DEBUG_POSTFIX "-dbg")
```
设置编译优化

用来区分debug模式，设置debug模式下的后缀，使用debug模式编译时，生成的库名啥的都会带上-dbg后缀

---

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
```

就是很常规的，没啥讲的

---

```cmake
# installation destinations
# must set before add_subdirectory
if(UNIX OR VXWORKS)
    include(GNUInstallDirs)
    set(MY_LIBRARY_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(MY_LIBRARY_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
    set(MY_LIBRARY_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/${MY_LIB_NAME}/cmake")
    set(MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${MY_LIB_NAME}")
elseif(WIN32)
    set(MY_LIBRARY_INC_INSTALL_DIR "include")
    set(MY_LIBRARY_RUNTIME_INSTALL_DIR   "bin")
    set(MY_LIBRARY_LIBRARY_INSTALL_DIR   "bin")
    set(MY_LIBRARY_ARCHIVE_INSTALL_DIR   "lib")
    set(MY_LIBRARY_FRAMEWORK_INSTALL_DIR "bin")
    set(MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR "share/${MY_LIB_NAME}/cmake")
    set(MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR "share/${MY_LIB_NAME}")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else()
    message(FATAL_ERROR "Could not set install folders for this platform!")
endif()
```

就用用来跨平台定义好安装位置的目录，这段代码必须要在add_subdirectory之前，因为子目录要用这些变量

unix下需要使用`include(GNUInstallDirs)`导入变量

默认的变量：

- CMAKE_INSTALL_BINDIR=bin
- CMAKE_INSTALL_INCLUDEDIR=include
- CMAKE_INSTALL_LIBDIR=lib
- CMAKE_INSTALL_DATADIR=share

实际安装时，会有一个前缀`CMAKE_INSTALL_PREFIX`，这个在linux下默认时`/usr/local`

---

```cmake
# set default build control
option(BUILD_EXAMPLES "whether or not to build the examples" ON)
option(BUILD_TESTS "whether or not to build the tests" ON)
option(BUILD_TOOLS "whether or not to build the tests" ON)
# add subdiretories
add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
if(BUILD_TOOLS)
    enable_testing()
    add_subdirectory(tool)
endif()
```

用来设置是否编译示例和测试用例，用来配合后面的  `add_subdirectory(example)`

这个可以在编译时就 `-DBUILD_EXAMPLES=OFF` 进行更改设置，来设置不编译example目录

`enable_testing()` 是用来使能cmake的测试功能，不过平时都是用google test多一些

---

```cmake
# install readme and license
install(FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
  "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
  DESTINATION ${MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR}
)
```

用来安装LICENSE文件和README.md文件，一般是安装到`/usr/local/share`下

---

```cmake
# install generate cmake files
install(EXPORT ${MY_LIB_NAME}_targets DESTINATION ${MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR}
                                       FILE ${MY_LIB_NAME}-config.cmake)
```

这句话很重要，这个需要放到`add_subdirectory(src)`下面，因为 **${MY_LIB_NAME}_targets** 是在src目录下的cmake文件中配置的，这里是把这个配置到出到文件 **${MY_LIB_NAME}-config.cmake** ，这样就可以使用**find_package**找到库了

---
