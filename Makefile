#
# Makefile
#
CC 				= aarch64-linux-gcc
LVGL_DIR_NAME 	?= lvgl
LVGL_DIR 		?= .

WARNINGS		:= -Wall -std=c99 -D_DEFAULT_SOURCE
CFLAGS 			?= -O3 -g0 -I$(LVGL_DIR)/ $(WARNINGS) -I$(LVGL_DIR)/ui
LDFLAGS 		?= -lm
BIN 			= demo
BUILD_DIR 		= ./build
BUILD_OBJ_DIR 	= $(BUILD_DIR)/obj
BUILD_BIN_DIR 	= $(BUILD_DIR)/bin

prefix 			?= /usr
bindir 			?= $(prefix)/bin

#Collect the files to compile
MAINSRC = 		./main.c	\
				./Login.c	\
				./Draw.c	\
				./Photo.c	\
				./Seeting.c

#include $(LVGL_DIR)/lvgl/lvgl.mk
#include $(LVGL_DIR)/lv_drivers/lv_drivers.mk

CSRCS 			+=$(LVGL_DIR)/mouse_cursor_icon.c
CSRCS 			+=$(shell find $(LVGL_DIR)/lvgl/src -name "lv_snapshot.c")
CSRCS 			+=$(LVGL_DIR)/chinese.c

CSRCS			+=$(shell find $(LVGL_DIR)/ui -name "*.c")

OBJEXT 			?= .o

AOBJS 			= $(ASRCS:.S=$(OBJEXT))
COBJS 			= $(CSRCS:.c=$(OBJEXT))

MAINOBJ 		= $(MAINSRC:.c=$(OBJEXT))

SRCS 			= $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS 			= $(AOBJS) $(COBJS) $(MAINOBJ)
TARGET 			= $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

## MAINOBJ -> OBJFILES

all: default

$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c  $< -o $@
	@echo "CC $<"
    
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
