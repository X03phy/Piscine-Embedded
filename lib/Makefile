# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/08 13:20:09 by x03phy            #+#    #+#              #
#    Updated: 2025/11/08 13:22:05 by x03phy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==== Library Configuration ====================================================

NAME = lib-embedded.a

SRC_DIR = src
BUILD = .build
INCLUDE = -Iinclude



#==== Tools ====================================================================

CC = avr-gcc
AR = avr-ar
ARFLAGS = rcs



#==== Compilation Flags ========================================================

MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200

CFLAGS = -Wall -Wextra -Werror -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -MMD $(INCLUDE)

NAME=lib-embedded.a



#==== Files ====================================================================

SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))
DEP = $(OBJ:%.o=%.d)



#==== Targets ==================================================================

all: create_dir $(NAME)

create_dir:
	@mkdir -p $(BUILD)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if [ -d $(BUILD) ]; then rm -rf $(BUILD) && printf "\033[1;31mDeleted: $(BUILD)\033[0m\n"; fi

fclean: clean
	@if [ -f $(NAME) ]; then rm -f $(NAME) && printf "\033[1;31mDeleted: $(NAME)\033[0m\n"; fi

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re create_dir
