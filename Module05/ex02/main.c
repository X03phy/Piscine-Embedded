/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:04:20 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/05 16:43:07 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://ece-classes.usc.edu/ee459/library/documents/ADC.pdf

#include <avr/io.h>
#include <util/delay.h>

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

static void uart_tx( char c )
{
	// UDRE0 : USART Data Register Empty
	while ( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}

static void uart_printdec( uint16_t dec )
{
	char buf[5]; // max 1023 = 4 digits
	int i = 0;

	if ( dec == 0 )
	{
		uart_tx( '0' );
		return;
	}

	while ( dec > 0 )
	{
		buf[i++] = '0' + ( dec % 10 );
		dec /= 10;
	}

	for ( int j = i - 1; j >= 0; j-- )
		uart_tx( buf[j] );
}

static void adc_init( void )
{
	// Set ADC reference to AVCC and enable 10-bit resolution
	ADMUX = ( 1 << REFS0 );
	// Enable ADC and set prescaler to 128 (for 16MHz -> 125kHz ADC clock)
	ADCSRA = ( 1 << ADEN ) | ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );

	// Configure PC0, PC1, PC2 as inputs
	DDRC &= ~ ( ( 1 << PC0 ) | ( 1 << PC1 ) | ( 1 << PC2 ) );
	// Disable digital input buffers
	DIDR0 = ( 1 << ADC0D ) | ( 1 << ADC1D ) | ( 1 << ADC2D );
}

static uint16_t adc_read( uint8_t channel )
{
	// Select channel
	ADMUX = ( ADMUX & 0xF0 ) | ( channel & 0x0F );

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
	uart_init();
	adc_init();

	while ( 1 )
	{
		uart_printdec( adc_read( 0 ) );
		uart_tx( ',' ); uart_tx( ' ' );
		uart_printdec( adc_read( 1 ) );
		uart_tx( ',' ); uart_tx( ' ' );
		uart_printdec( adc_read( 2 ) );
		uart_tx( '\r' ); uart_tx( '\n' );

		_delay_ms( 20 );
	}

	return ( 0 );
}
