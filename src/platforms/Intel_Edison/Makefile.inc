CFLAGS += -DBBB -Wno-format
LDFLAGS += -L/usr/lib/lusb # this is the default location for libusb, installed by opkg 
SRC += 	gpio.c
