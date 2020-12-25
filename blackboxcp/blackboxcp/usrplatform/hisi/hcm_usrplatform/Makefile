#
# lib Makefile
#

CFLAGS = -Wall  -I../../hci_inc -I./ -I../../com_inc
ARFLAGS_SO += -shared -fPIC -o
REL_LIB = ../lib/
REL_NFSLIB =  ../../../../../../nfs/hcsdk/
REL_LINKLIB =  ../../../../ipc-mainproject/lib/HI_3516DV300/so/

export CROSS:=arm-himix200-linux-
CC	= @echo "hisi GCC	$@"; $(CROSS)gcc
AR	= @echo "hisi AR	$@"; $(CROSS)ar
TP  = @echo "hisi TP	$@"; $(CROSS)strip


SRC  := $(wildcard *.c)
OBJ  := $(SRC:%.c=%.o)

TARGET = libusrplatform.so


all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

.PHONY: clean

$(TARGET): %: $(OBJ) 
	$(CC) $(CFLAGS) $(ARFLAGS_SO) $@ $^ -lpthread 
	@cp $(TARGET) $(REL_LIB)
	cp $(TARGET) $(REL_NFSLIB)
	cp $(TARGET) $(REL_LINKLIB)

clean:
	rm -f *.o *.so
