# CMAKE LIB TEMPLATE

标准cmake工程用来构建自己的库

## 文件目录结构

```plaintext
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── cmake
│   └── help.cmake
├── docs
│   ├── src下cmake分析.md
│   └── 根目录cmake分析.md
├── example
│   ├── CMakeLists.txt
│   └── example_main.cc
├── include
│   └── mytest
│       ├── detail
│       │   └── add.h
│       └── mytest.h
├── src
│   ├── CMakeLists.txt
│   ├── config.hpp.in
│   ├── detail
│   │   └── add.cc
│   └── mytest.cc
├── test
│   ├── CMakeLists.txt
│   ├── test_add.cc
│   └── test_main.cc
└── tool
    ├── CMakeLists.txt
    ├── tool_example.py
    └── tool_example.sh
```

- cmake 放一些cmake函数
- docs **放了配置对应的文档**
- example 放工程的示例和demo
- include 放工程的头文件
- src 放工程的源文件
- test 放工程的测试案例文件，保障代码的功能性
- tool 放工程的一些小工具

总体靠根目录下的`CMakeLists.txt`文件构建起来

## 使用

```bash
cmake -S . -B build -DBUILD_EXAMPLES=ON -DBUILD_TESTS=ON -DBUILD_TOOLS=ON
cmake --build build
sudo cmake --install build
```

编译：
```plaintext
[ 12%] Building CXX object src/CMakeFiles/mylib.dir/mytest.cc.o
[ 25%] Building CXX object src/CMakeFiles/mylib.dir/detail/add.cc.o
[ 37%] Linking CXX shared library libmylib.0.1.2.dylib
[ 37%] Built target mylib
[ 50%] Building CXX object example/CMakeFiles/example.dir/example_main.cc.o
[ 62%] Linking CXX executable example
[ 62%] Built target example
[ 75%] Building CXX object test/CMakeFiles/test_main.dir/test_main.cc.o
[ 87%] Building CXX object test/CMakeFiles/test_main.dir/test_add.cc.o
[100%] Linking CXX executable test_main
[100%] Built target test_main
```

安装时提示：
```plaintext
-- Install configuration: "Release"
-- Up-to-date: /usr/local/share/mylib/LICENSE
-- Installing: /usr/local/share/mylib/README.md
-- Installing: /usr/local/lib/mylib/cmake/mylib-config.cmake
-- Installing: /usr/local/lib/mylib/cmake/mylib-config-release.cmake
-- Installing: /usr/local/lib/libmylib.0.1.2.dylib
-- Installing: /usr/local/include/mylib/config_impl.hpp
-- Up-to-date: /usr/local/include/mylib/mytest.h
-- Up-to-date: /usr/local/include/mylib/detail/add.h
-- Installing: /usr/local/lib/mylib/cmake/mylib-config-version.cmake
-- Up-to-date: /usr/local/bin/tool_example.sh
-- Up-to-date: /usr/local/bin/tool_example.py
```

如果使用debug模式编译，那么库名会带上-dbg后缀

cmake使用库时：
```cmake
find_package(mylib)
target_link_libraries(demo mylib)
```
默认安装的头文件已经在/usr/local/include/
也不需要设置include_directories

## Ref

- [https://github.com/foonathan/memory/tree/main](https://github.com/foonathan/memory/tree/main)
- [https://murphypei.github.io/blog/2018/11/cmake-install-find-package](https://murphypei.github.io/blog/2018/11/cmake-install-find-package)
