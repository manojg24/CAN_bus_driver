TARGET  = Driver
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS  = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -O0 -g -Wall
CFLAGS += -DSTM32F446xx
CFLAGS += -T stm32f446re.ld
CFLAGS += -nostartfiles -nodefaultlibs

SRCS    = main.c

all: $(TARGET).bin

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(TARGET).bin verify reset exit 0x08000000"

clean:
	rm -f *.elf *.bin
