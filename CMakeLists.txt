cmake_minimum_required(VERSION 3.1)

# 1. 交叉编译器设置
set(CMAKE_C_COMPILER /opt/toolchains/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/toolchains/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++)

project(sq_lv2)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# 2. 编译选项 (确保全局定义 LV_USE_SJPG 和 LV_USE_FS_POSIX)
set(CMAKE_C_STANDARD 99)
add_compile_options(-O3 -g0 -Wall -D_DEFAULT_SOURCE -DLV_CONF_INCLUDE_SIMPLE -DLV_USE_SJPG=1 -DLV_USE_FS_POSIX=1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

# Build LVGL from the checked-in sources so the RK1808 binary is self-contained.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# 3. 头文件路径
include_directories(
    .
    ./ui
    ./lv_drivers
    /home/gec/aarch64-libs/include
)

link_directories(
    /home/gec/aarch64-libs/lib
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(lvgl EXCLUDE_FROM_ALL)
add_subdirectory(lv_drivers)

# 4. 搜寻 UI 目录下的 C 文件
file(GLOB_RECURSE UI_SRCS CONFIGURE_DEPENDS "ui/*.c")

# 🎯【关键改动】：强行把 LVGL 的 SJPG 解码器 C 源码文件找出来，编译进 demo 中
file(GLOB_RECURSE LVGL_SJPG_SRCS "lvgl/src/extra/libs/sjpg/*.c")

# 5. 生成 demo 可执行文件（包含了 LVGL_SJPG_SRCS）
add_executable(demo main.c ${UI_SRCS} ${LVGL_SJPG_SRCS})

# 6. 链接库设置
target_link_libraries(demo
    PRIVATE
        lvgl::drivers
        lvgl
        
        -Wl,-Bstatic
        curl
        ssl
        crypto
        z
        
        -Wl,-Bdynamic
        pthread
        m
        dl
)
