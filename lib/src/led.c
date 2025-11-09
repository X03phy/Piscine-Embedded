/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:55:41 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/08 19:19:53 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "led.h"
#include "utils.h"

/*  Doc  */
void init_led_D1( void )
{
	_SET( LED_DDR, LED_D1_PIN ) ;
	_CLEAR( LED_PORT, LED_D1_PIN ) ;
}

void init_led_D2( void )
{
	_SET( LED_DDR, LED_D2_PIN ) ;
	_CLEAR( LED_PORT, LED_D2_PIN ) ;
}

void init_led_D3( void )
{
	_SET( LED_DDR, LED_D3_PIN ) ;
	_CLEAR( LED_PORT, LED_D3_PIN ) ;
}

void init_led_D4( void )
{
	_SET( LED_DDR, LED_D4_PIN ) ;
	_CLEAR( LED_PORT, LED_D4_PIN ) ;
}

void init_leds( void )
{
	init_led_D1() ;
	init_led_D2() ;
	init_led_D3() ;
	init_led_D4() ;
}
