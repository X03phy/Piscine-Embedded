/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:04:20 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/05 16:46:05 by x03phy           ###   ########.fr       */
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

static void uart_printdec( int16_t dec )
{
	char buf[5]; // max 1023 = 4 digits
	int i = 0;
	int negative = 0;

	if ( dec == 0 )
	{
		uart_tx( '0' );
		return;
	}

	if ( dec < 0 )
	{
		negative = 1;
		dec = -dec;
	}

	while ( dec > 0 )
	{
		buf[i++] = '0' + ( dec % 10 );
		dec /= 10;
	}

	if ( negative )
		uart_tx('-');

	for ( int j = i - 1; j >= 0; j-- )
		uart_tx( buf[j] );
}

static void adc_init( void )
{
	// Set ADC reference to AVCC and enable 10-bit resolution
	ADMUX = ( 1 << REFS1 ) | ( 1 << REFS0 ) | ( 1 << MUX3 ); // Internal 1.1V ref + ADC8
	// Enable ADC and set prescaler to 128 (for 16MHz -> 125kHz ADC clock)
	ADCSRA = ( 1 << ADEN ) | ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );

	_delay_ms( 10 );
}

static uint16_t adc_read( void )
{
	// Start conversion
	ADCSRA |= ( 1 << ADSC );
	
	// Wait for conversion to complete
	while ( ADCSRA & ( 1 << ADSC ) );
	
	return ( ADC ); // 10-bit result
}

// LDR (Light Dependent Resistor)
// NTC (Negative Temperature Coefficient)

int main( void )
{
	uint16_t raw;
	int32_t temp;

	uart_init();
	adc_init();

	while ( 1 )
	{
		raw = adc_read(); // channel 8
		temp = ( ( ( int32_t ) raw ) * 100 - 32431 ) / 122;
		uart_printdec( temp );
		uart_tx('\r'); uart_tx('\n');
		_delay_ms( 20 );
	}

	return ( 0 );
}
