
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE=arm-none-linux-gnueabi-
endif

ifeq ($(BOARD),dcts)
RAM_BASE=0x04000000
endif

ifeq ($(BOARD),uemd)
RAM_BASE=0x00100000  # IM0 bank
endif

ifeq ($(BOARD),)
$(error "Please specify the board type: make BOARD=<uemd/dcts> <target>")
endif

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump 

.PHONY: clean all dump

CFLAGS = -nostdinc \
		-fno-common \
		-fno-builtin \
		-ffixed-r8 \
		-msoft-float \
		-ffreestanding \
		-isystem /usr/lib/gcc/arm-module2-linux-gnueabi/4.4.0/include \
		-I./ \
		-DTEXT_BASE=$(RAM_BASE) \
		-Wall -marm -mcpu=arm1176jzf-s


ifneq ($(DEBUG),)
CFLAGS += -g
endif

all: main.bin
	
start.o: start.S
	$(CC) $(CFLAGS) -c -o start.o start.S
	
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c 
	
main.elf: start.o main.o
	$(LD) -T boot.lds -Ttext=$(RAM_BASE) main.o -o main.elf

main.bin: main.elf
	$(OBJCOPY) -v -O binary main.elf main.bin

clean:
	rm -f *.a
	rm -f *.o 
	rm -f *.elf
	rm -f *.v
	rm -f *.bin
	rm -f *.md5
	rm -f *.hex
	rm -f *.dmp

dump: main.elf
	$(OBJDUMP) -D main.elf

