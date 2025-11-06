/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:58:24 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/06 13:03:19 by ebonutto         ###   ########.fr       */
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

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

static int ft_toupper( int c )
{
	if ( 'a' <= c && c <= 'z' )
		return ( c - 32 );
	return ( c );
}

static int ft_strlen( const char *s )
{
	int i;

	i = 0;
	while ( s[i] )
		i += 1;

	return ( i );
}

static int ft_strcmp( const char *s1, const char *s2 )
{
	int i;

	if ( !s1 || !s2 )
		return ( 0 );

	i = 0;
	while ( s1[i] && s1[i] == s2[i] )
		i += 1;
	
	return ( s1[i] - s2[i] );
}

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

static void uart_init( void )
{
	// Configure USART Baud Rate Registers
	UBRR0H = BAUD_PRESCALE >> 8;
	UBRR0L = BAUD_PRESCALE;

	// Configure USART Control and Status Register
	// TXEN0 : Transmitter Enable bit
	// RXEN0 : Receiver Enable bit
	UCSR0A = ( 1 << U2X0 ); // Double Transmission Speed
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ); // TX, RX
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

static void uart_printstr( const char* str )
{
	while ( *str )
		uart_tx( *str++ );
}

static int parse_input( const char *input )
{
	if ( ft_strlen( input ) != 7 )
		return ( 0 );

	if ( input[0] != '#' )
		return ( 0 );

	for ( int i = 1; input[i]; i += 1 )
	{
		if ( ! ( ( '0' <= input[i] && input[i] <= '9' ) ||
		         ( 'A' <= input[i] && input[i] <= 'F' ) ) )
			return ( 0 );
	}

	return ( 1 );
}

static char hex_to_byte( char *s, int i )
{
	if ( 'A' <= s[i] && s[i] <= 'Z' )
		s[i] -= 7;
	if ( 'A' <= s[i + 1] && s[i + 1] <= 'Z' )
		s[i + 1] -= 7;

	return ( ( ( s[i] - 48 ) << 4 ) | ( s[i + 1] - 48 ) );
}

int main( void )
{
	char c;
	char buffer[8];
	uint8_t idx;
	uint8_t r, g, b;

	init_rgb();
	uart_init();

	uart_printstr( "Enter your color (#RRGGBB): " );

	idx = 0;
	while ( 1 )
	{
		c = uart_rx();

		if ( c == 127 || c == 8 ) // Delete
		{
			if ( idx > 0 )
			{	
				idx -= 1;
				uart_printstr( "\b \b" );
			}
		}
		else if ( c == '\r' || c == '\n' )
		{
			uart_tx( '\r' ); uart_tx( '\n' );
			buffer[idx] = '\0';
			idx = 0;

			if ( parse_input( buffer ) )
			{
				r = hex_to_byte( buffer, 1 );
				g = hex_to_byte( buffer, 3 );
				b = hex_to_byte( buffer, 5 );
				set_rgb( r, g, b );
				uart_printstr( "Enter your color (#RRGGBB): " );
			}
			else
			{
				uart_printstr( "Invalid format. Use #RRGGBB\r\n\r\n" );
				uart_printstr( "Enter your color (#RRGGBB): " );
				buffer[0] = '\0';
				idx = 0;
			}
		}
		else if ( idx < 7 )
		{
			c = ft_toupper( c );
			uart_tx( c );
			buffer[idx++] = c;
		}
	}

	return ( 0 );
}
