/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:40:10 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/05 19:21:25 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PWM (Pulse Width Modulation)

#include <avr/io.h>
#include <util/delay.h>

ISR( TIMER0_COMPA_vect )
{
	// Met à jour la valeur du PWM
	pwm_value += direction;

	// Inversion de direction
	if (pwm_value == MAX_PWM || pwm_value == 0)
		direction = -direction;

	// Met à jour le rapport cyclique du Timer1
	OCR1A = pwm_value;
}

int main( void )
{
	DDRB |= ( 1 << PB1 ); // Configure PB1 as an output

	cli();
	TCCR1A = ( 1 << COM1A1 ) | ( 1 << WGM10 );  // non-inverting, 8-bit Fast PWM
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );    // Fast PWM, prescaler = 1
	OCR1A = 0;                                  // initial PWM at 

	TCCR0A = 
	sei(); // interruptions globales

	while ( 1 );

	return ( 0 );
}
