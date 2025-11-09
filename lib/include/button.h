/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:44:55 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/08 19:19:27 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>
#include <util/delay.h>

/* Macros */

/*  Doc  */
#define SW1_PORT    PORTD
#define SW1_DDR     DDRD
#define SW1_PIN_REG PIND
#define SW1_PIN     PD2

/*  Doc  */
#define SW2_PORT    PORTD
#define SW2_DDR     DDRD
#define SW2_PIN_REG PIND
#define SW2_PIN     PD4

/*  Doc  */
#define BUTTON_PRESSED  0
#define BUTTON_RELEASED 1

/*  Doc  */
#define DEBOUNCE_DELAY 20



/* Prototypes */

/*  Doc  */
void init_button_sw1( void );

/*  Doc  */
void init_button_sw2( void );

/*  Doc  */
void init_buttons( void );

/*  Doc  */
uint8_t read_button_sw1( void );

/*  Doc  */
uint8_t read_button_sw2( void );

/*  Doc  */
uint8_t is_sw1_pressed( void );

/*  Doc  */
uint8_t is_sw2_pressed( void );

/*  Doc  */
void debounce_button( void );



#endif
