MMCU=atmega328p
PORT=/dev/ttyUSB0
CFLAGS=-mmcu=${MMCU} -Os

SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

prep:
	mkdir -p build
all: prep
	avr-gcc ${SOURCES} ${CFLAGS} -o build/out.elf
	avr-objcopy -O ihex build/out.elf build/out.hex
flash: all
	avrdude -p ${MMCU} -P ${PORT} -c arduino -U flash:w:build/out.hex -b 57600