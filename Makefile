MMCU=atmega328p
PORT=/dev/ttyACM0
CFLAGS=-mmcu=${MMCU} -Os

SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

prep:
	mkdir -p build
all: prep
	avr-gcc ${SOURCES} ${CFLAGS} -o build/out.elf -Xlinker -Map=output.map
	avr-objcopy -O ihex build/out.elf build/out.hex
flash: all
	avrdude -p ${MMCU} -P ${PORT} -c arduino -D -U flash:w:build/out.hex
dev: flash
	sudo cat $(PORT) 9600