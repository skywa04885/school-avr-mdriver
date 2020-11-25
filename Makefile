# T100 - Makefile Motor Driver
# 	Author: Luke Rieff

# Configuration
MCU 		:= atmega328p					# Device type
DEV_PTH := /dev/ttyUSB0				# Physical device location
PROGRM	:= arduino						# AVRDUDE programmer
BAUD		:= 9600								# USART Baud
F_CPU		:= 16000000UL					# Processor speed ( M328: +/- 16Mhz )
MOD_DIR	:= src/sources/

# Binaries
GCC 			:= /bin/avr-gcc
AS				:= /bin/avr-as
LD				:= /bin/avr-ld
AVRDUDE		:= /bin/avrdude
SIZE			:= /bin/avr-size
OBJCOPY		:= /bin/avr-objcopy

# GCC Args
GCC_ARGS	+= -DF_CPU=$(F_CPU)
GCC_ARGS	+= -DBAUD=$(BAUD)
GCC_ARGS	+= -mmcu=$(MCU)
GCC_ARGS 	+= -Os
GCC_ARGS	+= -fdata-sections
GCC_ARGS	+= -ffunction-sections
GCC_ARGS	+= -Wl,--gc-sections

# OBJCopy args
OBJC_ARGS	+= -O ihex

# Assembly Arguments
AS_ARGS		+= -mmcu=$(MCU)

# Size Args
SIZE_ARGS	+= --mcu=$(MCU)
SIZE_ARGS += --format=avr

# AVRDUDE Args
AVD_ARGS	+= -P $(DEV_PTH)
AVD_ARGS	+= -p $(MCU)
AVD_ARGS	+= -c $(PROGRM)

# Source files
MODULES		+= $(MOD_DIR)include.o
MODULES		+= $(MOD_DIR)io.o
MODULES		+= $(MOD_DIR)joystick.o
MODULES		+= $(MOD_DIR)stepper.o
MODULES		+= $(MOD_DIR)usart.o
MODULES		+= $(MOD_DIR)commands.o

# Make rules for C source files
%.o: %.c
	$(GCC) $(GCC_ARGS) $< -c -o $@

# Make rules for assembly source files
%.o: %.s
	$(AS) $(AS_ARGS) $< -o $@

# Compiles everything
all: $(MODULES)
	$(GCC) $(GCC_ARGS) main.c $(MODULES) -o main.elf

# Displays the binary size
size:
	$(SIZE) $(SIZE_ARGS) main.elf

# Generates the intel hex file
ihex:
	$(OBJCOPY) $(OBJC_ARGS) main.elf main.ihex

# Flashes to the device
flash: all size ihex
	$(AVRDUDE) $(AVD_ARGS) -U flash:w:main.ihex

# Cleans the project
clean:
	rm -rf *.o *.elf *.ihex
	rm -rf $(MOD_DIR)*.o
