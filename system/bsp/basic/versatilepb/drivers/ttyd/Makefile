include ../../../../../make.rule

BUILD_DIR = ../../../../../build
TARGET_DIR = $(BUILD_DIR)/rootfs

LDFLAGS = -L$(BUILD_DIR)/lib -lnosys

CFLAGS += -I $(BUILD_DIR)/include

TTYD_OBJS = ttyd.o

TTYD = $(TARGET_DIR)/drivers/versatilepb/ttyd

$(TTYD): $(TTYD_OBJS) \
		$(BUILD_DIR)/lib/libewokc.a
	$(LD) -Ttext=100 $(TTYD_OBJS) -o $(TTYD) $(LDFLAGS) -lewokc -lc

clean:
	rm -f $(TTYD_OBJS) $(TTYD)
