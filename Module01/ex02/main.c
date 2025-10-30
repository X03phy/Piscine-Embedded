/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:13:04 by x03phy            #+#    #+#             */
/*   Updated: 2025/10/30 17:02:52 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main( void )
{
	DDRB |= ( 1 << PB1 ); // Configure PB1 as an output

	TCCR1A |= ( 1 << COM1A1 ); // For duty cycle
	TCCR1A |= ( 1 << WGM11 );
	TCCR1B |= ( 1 << WGM12 ) | ( 1 << WGM13 ); // mode 14 Fast PWM

	TCCR1B |= ( 1 << CS12 ) | ( 1 << CS10 ); // Prescaler = 1024
	ICR1 = 15625; // 1 cycle
	OCR1A = 1563; // Duty cycle 10%

	while ( 1 ); // Infinite loop

	return ( 0 );
}
