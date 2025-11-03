/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:48:56 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/03 11:46:44 by ebonutto         ###   ########.fr       */
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
	UCSR0B = ( 1 << TXEN0 ); // Enable transmission
	UCSR0C = ( 1 << UCSZ00 ) | ( 1 << UCSZ01 ); // For character size (8 bits)
}

static void uart_tx( char c )
{
	// UDRE0 : USART Data Register Empty
	while ( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}

static void uart_printstr( const char* str )
{
	while ( *str )
		uart_tx( *str++ );
}

static void timer1_init( void )
{
	cli();
	TCCR1A = 0; // Normal mode
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS12 ) | ( 1 << CS10 ); // CTC mode, prescaler 1024
	OCR1A = 31250;
	// TIMSK1 : Timer/Counter1 Interrupt Mask Register
	// OCIE1A : Output Compare A Match Interrupt Enable
	TIMSK1 = ( 1 << OCIE1A );
	sei();
}

ISR( TIMER1_COMPA_vect )
{
	uart_printstr( "Hello World!\r\n" );
}

int main( void )
{
	uart_init();
	timer1_init();

	while ( 1 );

	return ( 0 );
}
