# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 14:18:07 by ebonutto          #+#    #+#              #
#    Updated: 2025/11/01 16:48:43 by x03phy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==== Main Options =============================================================

# The target microcontroller (MCU) model
MCU = atmega328p

# The clock frequency used by the MCU
F_CPU = 16000000UL

# Communication
BAUD = 115200

# The main output file name (without extension)
TARGET = main



#==== Variables ================================================================

REMOVE = rm -f
REMOVEDIR = rm -rf

RED = \033[0;31m
GRN = \033[0;32m
NC = \033[0m # No Color



#==== Compile Options ==========================================================

# AVR GCC compiler
CC = avr-gcc

# Tool to convert ELF binary → HEX format
OBJCOPY = avr-objcopy

# Tool used to flash the program into the microcontroller
AVRDUDE = avrdude

# Compiler options for architecture and frequency
TARGET_ARCH = -mmcu=$(MCU)
TARGET_FREQ = -DF_CPU=$(F_CPU)
TARGET_BAUD = -DBAUD=$(BAUD)

# Serial port used to communicate with the MCU
TARGET_PORT = /dev/ttyUSB0



#==== Targets ==================================================================

.PHONY: all hex flash clean

all: hex flash

# Build the hex file
# -O ihex : Output format = Intel HEX (standard format to flash microcontrollers)
hex:
	@echo "$(GRN)Compiling project...$(NC)"
	$(CC) -Os $(TARGET_ARCH) $(TARGET_FREQ) $(TARGET_BAUD) -o $(TARGET).bin $(TARGET).c
	$(OBJCOPY) -O ihex $(TARGET).bin $(TARGET).hex
	@echo "$(GRN)HEX file generated: $(TARGET).hex$(NC)"

# Flash the MCU with AVRDUDE
# :i : Intel HEX format
flash: hex
	@echo "$(GRN)Flashing the microcontroller on $(TARGET_PORT)...$(NC)"
	$(AVRDUDE) -c arduino -p $(MCU) -P $(TARGET_PORT) -e -U flash:w:$(TARGET).hex:i
	@echo "$(GRN)Flash completed$(NC)"

clean:
	@echo "$(GRN)Cleaning up...$(NC)"
	$(REMOVE) $(TARGET).hex $(TARGET).bin
	@echo "$(GRN)Directory cleaned$(NC)"
