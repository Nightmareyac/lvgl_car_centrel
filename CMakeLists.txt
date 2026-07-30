cmake_minimum_required(VERSION 3.1)

# 1. 指定降级后的交叉编译器绝对路径（解决 GLIBC_2.34 报错）
set(CMAKE_C_COMPILER /opt/toolchains/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/toolchains/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++)

# 2. 项目名称
project(sq_lv2)

# 3. 语言标准与编译参数
set(CMAKE_C_STANDARD 99)
add_compile_options(-O3 -g0 -Wall -D_DEFAULT_SOURCE)

# 4. 指定最终生成的可执行文件 (demo) 输出路径到 build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

# 5. 头文件搜索路径
include_directories(
    .
    ./ui
    ./lv_drivers
    /home/gec/aarch64-libs/include
)

link_directories(/home/gec/aarch64-libs/lib)

# 6. 引入 lv_drivers 驱动层
add_subdirectory(lv_drivers)

# 7. 自动搜寻 ui/ 目录下的所有 .c 文件
file(GLOB_RECURSE UI_SRCS "ui/*.c")

# 8. 动态库搜索路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 9. 生成 demo 可执行文件
add_executable(demo main.c ${UI_SRCS})

# 10. 链接依赖库
target_link_libraries(demo
    PRIVATE
        lvgl::drivers   # 对应 lv_drivers 规则
        lvgl            # 对应 -llvgl 动态库
        pthread         # 对应 -lpthread
        m               # 对应 -lm
        curl
        ssl
        crypto
        z
        dl
)