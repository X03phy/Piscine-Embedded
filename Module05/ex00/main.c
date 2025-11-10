/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:04:20 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/10 09:16:10 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf

#include <avr/io.h>
#include <util/delay.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

const char *hex_base = "0123456789ABCDEF";

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

static void uart_tx( char c )
{
	// UDRE0 : USART Data Register Empty
	while ( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}

static void uart_printhex( uint8_t hex )
{
	uart_tx( hex_base[( hex >> 4 ) & 0x0F] );
	uart_tx( hex_base[hex & 0x0F] );

	uart_tx( '\r' );
	uart_tx( '\n' );
}

static void adc_init( void )
{
	// Set ADC reference to AVCC and enable 8-bit resolution
	ADMUX = ( ( 1 << REFS0 ) | ( 1 << ADLAR ) );
	// Enable ADC and set prescaler to 128 (for 16MHz -> 125kHz ADC clock)
	ADCSRA = ( 1 << ADEN ) | ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );

	// Configure PC0 as input
	DDRC &= ~ ( 1 << PC0 );
	// Disable digital input buffer
	DIDR0 = ( 1 << ADC0D );

	_delay_ms( 10 );

	ADCSRA |= ( 1 << ADSC );
	while ( ADCSRA & ( 1 << ADSC ) );
}

static uint8_t adc_read( void )
{
	// Start conversion
	ADCSRA |= ( 1 << ADSC );
	
	// Wait for conversion to complete
	while ( ADCSRA & ( 1 << ADSC ) );
	
	return ( ADCH ); // 8-bit result
}

int main( void )
{
	uint8_t value;

	uart_init();
	adc_init();

	while ( 1 )
	{
		value = adc_read();
		uart_printhex( value );
		_delay_ms( 20 );
	}

	return ( 0 );
}
