/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:19:58 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/08 21:01:34 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "led.h"
#include "button.h"
#include "interrupt.h"
#include "isr.h"
#include <avr/interrupt.h>

volatile int unpressed = 1;

ISR_EXTERNAL_0
{
	DISABLE_INT0();

	if ( IS_INT0_FALLING_EDGE() )
	{
		LED_TOGGLE( LED_D1_MASK );
		SET_INT0_RISING_EDGE();
	}
	else
	{
		unpressed = 0;
	}

	ENABLE_INT0();
}

int main( void )
{
	init_led_D1();
	init_button_sw1();

	cli();
	SET_INT0_FALLING_EDGE();
	ENABLE_INT0();
	sei();

	while ( 1 )
	{
		debounce_button();
		if ( !unpressed )
		{
			unpressed = 1;
			SET_INT0_FALLING_EDGE();
		}
	}

	return ( 0 );
}
