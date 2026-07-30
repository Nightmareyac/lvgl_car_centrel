#
# Makefile
#
CC 				= /opt/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
LVGL_DIR_NAME 	?= lvgl
LVGL_DIR 		?= .

WARNINGS		:= -Wall -std=c99 -D_DEFAULT_SOURCE

# 【修改点 1】：在 CFLAGS 中追加了 -I./ui 和 -I./lv_drivers
# 作用：告诉编译器在遇到 #include "ui.h" 或 #include "lv_drivers/display/fbdev.h" 时，去这两个目录下找头文件
CFLAGS 			?= -O3 -g0 -I$(LVGL_DIR)/ -I./ui -I./lv_drivers $(WARNINGS)

# 【修改点 2】：在 LDFLAGS 中追加了 -lpthread
# 作用：POSIX 线程库，驱动和系统定时任务可能需要线程支持
LDFLAGS 		?= -lm -lpthread

BIN 			= demo
BUILD_DIR 		= ./build
BUILD_OBJ_DIR 	= $(BUILD_DIR)/obj
BUILD_BIN_DIR 	= $(BUILD_DIR)/bin

prefix 			?= /usr
bindir 			?= $(prefix)/bin

# 收集主程序文件
MAINSRC = 		./main.c

# 【修改点 3】：解开了 lv_drivers.mk 的 include 注释
# 作用：将 lv_drivers 官方自带的 Makefile 规则引入进来，自动把 fbdev.c, evdev.c 等源码加入 CSRCS 变量
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

# 【修改点 4】：新增了 ui 目录下 C 文件的自动搜寻规则
# 作用：利用 shell 和 find 命令，把 ui/ 文件夹下的所有 .c 文件追加到 CSRCS 变量中
CSRCS          += $(shell find ./ui -type f -name '*.c')

OBJEXT 			?= .o

AOBJS 			= $(ASRCS:.S=$(OBJEXT))
COBJS 			= $(CSRCS:.c=$(OBJEXT))

MAINOBJ 		= $(MAINSRC:.c=$(OBJEXT))

# 汇总所有需要编译的源文件与目标文件
SRCS 			= $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS 			= $(AOBJS) $(COBJS) $(MAINOBJ)
TARGET 			= $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

## MAINOBJ -> OBJFILES

all: default

# 编译 C 文件生成 .o 中间文件
$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c  $< -o $@
	@echo "CC $<"

# 链接所有的 .o 文件生成可执行文件 demo
default: $(TARGET)
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	$(CC) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS) -L ./ -llvgl

clean:
	rm -rf $(BUILD_DIR)

install:
	install -d $(DESTDIR)$(bindir)
	install $(BUILD_BIN_DIR)/$(BIN) $(DESTDIR)$(bindir)

uninstall:
	$(RM) -r $(addprefix $(DESTDIR)$(bindir)/,$(BIN))