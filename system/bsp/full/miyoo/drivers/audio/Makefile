include ../../../../../make.rule

BUILD_DIR = ../../../../../build
TARGET_DIR = $(BUILD_DIR)/rootfs

LDFLAGS = -L$(BUILD_DIR)/lib -lnosys

CFLAGS += -I $(BUILD_DIR)/include

MS_AUD_OBJS = audctrl.o pcm_lib.o miyoo-dais.o reg_ctrl.o

MS_AUD = $(TARGET_DIR)/drivers/miyoo/audctrl

$(MS_AUD): $(MS_AUD_OBJS) \
		$(BUILD_DIR)/lib/libewokc.a
	$(LD) -Ttext=100 $(MS_AUD_OBJS) -o $(MS_AUD) $(LDFLAGS) -lewokc -lc

clean:
	rm -f $(MS_AUD_OBJS) $(MS_AUD)