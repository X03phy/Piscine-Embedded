/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:48:56 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/01 18:24:30 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://marcjuneau.ca/?p=129

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

static void uart_init( void )
{	
	// Configure USART Baud Rate Registers
	UBRR0H = BAUD_PRESCALE >> 8; 
	UBRR0L = BAUD_PRESCALE;

	// Configure USART Control and Status Register
	// TXEN0 : Transmitter Enable bit
	// RXEN0 : Receiver Enable bit
	UCSR0A = ( 1 << U2X0 ); // Double Transmission Speed
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ); // Enable transmission and reception
	UCSR0C = ( 1 << UCSZ00 ) | ( 1 << UCSZ01 ); // For character size (8 bits)
}

static char uart_rx( void )
{
	// RXC0 : Receive Complete
	while ( ! ( UCSR0A & ( 1 << RXC0 ) ) );

	return ( UDR0 );
}

static void uart_tx( char c )
{
	// UDRE0 : USART Data Register Empty
	while ( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}

int main( void )
{
	char c;

	uart_init();

	while ( 1 )
	{
		c = uart_rx();
		uart_tx( c );
	}

	return ( 0 );
}
