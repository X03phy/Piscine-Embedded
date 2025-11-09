/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:50:05 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/08 19:05:51 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "button.h"

/*  Doc  */
void init_button_sw1(void)
{
	SW1_DDR &= ~(1 << SW1_PIN);
	SW1_PORT |= (1 << SW1_PIN);
}

/*  Doc  */
void init_button_sw2( void )
{
	SW2_DDR &= ~ ( 1 << SW2_PIN );
	SW2_PORT |= (1 << SW2_PIN);
}

/*  Doc  */
void init_buttons(void)
{
	init_button_sw1();
	init_button_sw2();
}

/*  Doc  */
uint8_t read_button_sw1(void)
{
	return ( ( SW1_PIN_REG & ( 1 << SW1_PIN ) ) ? BUTTON_RELEASED : BUTTON_PRESSED ) ;
}

/*  Doc  */
uint8_t read_button_sw2(void)
{
	return ( ( SW2_PIN_REG & ( 1 << SW2_PIN ) ) ? BUTTON_RELEASED : BUTTON_PRESSED ) ;
}

/*  Doc  */
uint8_t is_sw1_pressed(void)
{
	return ( ! ( SW1_PIN_REG & ( 1 << SW1_PIN ) ) ) ;
}

/*  Doc  */
uint8_t is_sw2_pressed( void )
{
	return ( ! ( SW2_PIN_REG & ( 1 << SW2_PIN ) ) ) ;
}

void debounce_button( void )
{
	_delay_ms( DEBOUNCE_DELAY );
}
