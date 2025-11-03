/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:58:24 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/03 11:30:39 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://forum.arduino.cc/t/pulsing-rgb-led-understanding-ocm-and-phase-correct-pwm/1212393

#include <avr/io.h>
#include <util/delay.h>

#define RED_BIT PD5 // Timer0 : OC0B
#define GRN_BIT PD6 // Timer0 : OC0A
#define BLU_BIT PD3 // Timer2 : OC2B

#define RGB_MASK ( ( 1 << RED_BIT ) | ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ) )

static void timer0_init( void )
{
	// Mode CTC (Clear Timer on Compare Match)
	TCCR1A = ( 1 << COM1A0 ); // Toggle OC1A at every comp
	TCCR1B = ( 1 << WGM12 );  // CTC mode

	// Controls PB1 through the timer
	OCR1A = 7813;
	TCCR1B |= ( 1 << CS12 ) | ( 1 << CS10 ); // Prescaler = 1024
}

static void init_rgb( void )
{
	DDRD |= RGB_MASK;
	timer1_init()
}

static void set_rgb( uint8_t r, uint8_t g, uint8_t b )
{
	
}

static void wheel( uint8_t pos )
{
	pos = 255 - pos;
	if ( pos < 85 ) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if ( pos < 170 )
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
	pos = pos - 170;
	set_rgb(pos * 3, 255 - pos * 3, 0);
}
}

int main( void )
{
	const uint8_t colors[] = {
		( 1 << RED_BIT ),                             // Red
		( 1 << GRN_BIT ),                             // Green
		( 1 << BLU_BIT ),                             // Blue
		( 1 << RED_BIT ) | ( 1 << GRN_BIT ),            // Yellow
		( 1 << GRN_BIT ) | ( 1 << BLU_BIT ),            // Cyan
		( 1 << RED_BIT ) | ( 1 << BLU_BIT ),            // Magenta
		( 1 << RED_BIT ) | ( 1 << GRN_BIT ) | ( 1 << BLU_BIT ) // White
	};
	const uint8_t color_count = sizeof(colors) / sizeof(colors[0]);

	init_rgb();
	sei();

	while ( 1 )
	{
		for ( uint8_t i = 0; i < color_count; i += 1 )
		{
			set_color( colors[i] );
			_delay_ms( 1000 );
		}
	}

	return ( 0 );
}
