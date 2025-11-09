# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 14:18:07 by ebonutto          #+#    #+#              #
#    Updated: 2025/11/08 14:11:53 by x03phy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==== Main Options =============================================================

MCU     = atmega328p
F_CPU   = 16000000UL
BAUD    = 115200
TARGET  = main



#==== Variables ================================================================

REMOVE     = rm -f
REMOVEDIR  = rm -rf

RED = \033[0;31m
GRN = \033[0;32m
NC  = \033[0m # No Color



#==== Compile Options ==========================================================

CC        = avr-gcc
OBJCOPY   = avr-objcopy
AVRDUDE   = avrdude

TARGET_ARCH = -mmcu=$(MCU)
TARGET_FREQ = -DF_CPU=$(F_CPU)
TARGET_BAUD = -DBAUD=$(BAUD)

TARGET_PORT = /dev/ttyUSB0

CFLAGS = -Werror -Wall -Wextra -Os -MMD $(TARGET_ARCH) $(TARGET_FREQ) $(TARGET_BAUD)



#==== Library (lib-embedded) ===================================================


LIB_DIR   = ../../lib
LIB_NAME  = -embedded
LIB_FILE  = $(LIB_DIR)/lib$(LIB_NAME).a
LIB_INC   = -I$(LIB_DIR)/include
LIB_LINK  = -L$(LIB_DIR) -l$(LIB_NAME)



#==== Source Files =============================================================

BUILD_DIR = .build

SRCS = main.c
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)


#==== Targets ==================================================================

.PHONY: all hex flash clean fclean re lib create_dir

all: lib hex flash



#==== Build Directory ==========================================================

create_dir:
	@mkdir -p $(BUILD_DIR)



#==== Build Library ============================================================

lib:
	@echo "$(GRN)Building embedded library...$(NC)"
	@$(MAKE) -C $(LIB_DIR) --no-print-directory



#==== Compilation ===============================================================

$(BUILD_DIR)/%.o: %.c | create_dir
	$(CC) -c $(CFLAGS) $(LIB_INC) $< -o $@



#==== Build HEX ================================================================

hex: $(OBJS)
	@echo "$(GRN)Compiling project...$(NC)"
	$(CC) $(CFLAGS) $(LIB_INC) -o $(TARGET).bin $(OBJS) $(LIB_LINK)
	$(OBJCOPY) -O ihex $(TARGET).bin $(TARGET).hex
	@echo "$(GRN)HEX file generated: $(TARGET).hex$(NC)"



#==== Flash MCU ================================================================

flash: hex
	@echo "$(GRN)Flashing the microcontroller on $(TARGET_PORT)...$(NC)"
	$(AVRDUDE) -c arduino -p $(MCU) -P $(TARGET_PORT) -e -U flash:w:$(TARGET).hex:i
	@echo "$(GRN)Flash completed$(NC)"



#==== Cleaning ================================================================

clean:
	@echo "$(RED)Cleaning up...$(NC)"
	@if [ -d $(BUILD_DIR) ]; then $(REMOVEDIR) $(BUILD_DIR) && echo "$(RED)\tDeleted: $(BUILD_DIR)$(NC)"; fi
	@echo "$(GRN)Objects removed$(NC)"

fclean: clean
	@$(MAKE) -C $(LIB_DIR) fclean --no-print-directory

re: fclean all



#==== Dependencies =============================================================

-include $(DEPS)
