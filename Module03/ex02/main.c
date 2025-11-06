/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:58:24 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/04 10:19:04 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://forum.arduino.cc/t/pulsing-rgb-led-understanding-ocm-and-phase-correct-pwm/1212393
// https://forum.arduino.cc/t/direct-pwm/37874
// https://fr.wikiversity.org/wiki/Micro_contr%C3%B4leurs_AVR/Le_Timer_0

#include <avr/io.h>
#include <util/delay.h>

#define RED_BIT PD5 // Timer0 : OC0B
#define GRN_BIT PD6 // Timer0 : OC0A
#define BLU_BIT PD3 // Timer2 : OC2B

#define RGB_MASK ( ( 1 << RED_BIT ) | ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ) )

static void timer0_init( void )
{
	// Fast pwd and non inverted on OC0A and OC0B
	TCCR0A = ( 1 << WGM00 ) | ( 1 << WGM01 ) | ( 1 << COM0A1 ) | ( 1 << COM0B1 );
	TCCR0B = ( 1 << CS00 ); // Prescaler = 1
}

static void timer2_init( void )
{
	// Fast pwd and non inverted on OC0A and OC2B
	TCCR2A = ( 1 << WGM20 ) | ( 1 << WGM21 ) | ( 1 << COM2B1 );
	TCCR2B = ( 1 << CS20 ); // No prescaler (perscaler = 1)
}

static void init_rgb( void )
{
	DDRD |= RGB_MASK;
	timer0_init();
	timer2_init();
}

static void set_rgb( uint8_t r, uint8_t g, uint8_t b )
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

static void wheel( uint8_t pos )
{
	pos = 255 - pos;

	if ( pos < 85 )
	{
		set_rgb( 255 - pos * 3, 0, pos * 3 );
	}
	else if ( pos < 170 )
	{
		pos = pos - 85;
		set_rgb( 0, pos * 3, 255 - pos * 3 );
	}
	else
	{
		pos = pos - 170;
		set_rgb( pos * 3, 255 - pos * 3, 0 );
	}
}

int main( void )
{
	uint8_t i;
	uint8_t r, g, b;

	init_rgb();

	while ( 1 )
	{
		for ( i = 0; i < 255; i += 1 )
		{
			wheel( i );
			_delay_ms( 20 );
		}
	}

	return ( 0 );
}
