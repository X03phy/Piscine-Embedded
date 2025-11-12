/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:15:29 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/12 23:29:20 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include<avr/eeprom.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

/*  UART  */
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

static void print_hex_value( char c )
{
	char hex[3];
	const char hexchars[] = "0123456789ABCDEF";

	hex[0] = hexchars[( c >> 4 ) & 0x0F];
	hex[1] = hexchars[c & 0x0F];
	hex[2] = '\0';
	uart_printstr( hex );
}

int main( void )
{
	uart_init();

	int i;
	uint16_t addr = 0;
	uint8_t byte_low, byte_high;
	char buf[10];

	while ( addr <= E2END )  // E2END = last EEPROM address
	{
		// Print address
		for ( i = 0; i < 7; i += 1 )
			buf[i] = "0123456789ABCDEF"[( addr >> ( 4 * ( 6 - i ) ) ) & 0x0F];
		buf[7] = '\0';
		uart_printstr( buf );

		// Print 8 bytes per line
		for ( i = 0; i < 8 && addr <= E2END; i += 1 )
		{
			uart_tx(' ');

			byte_low = eeprom_read_byte( ( uint8_t * ) addr );
			addr += 1;

			if ( addr <= E2END )
			{
				byte_high = eeprom_read_byte( ( uint8_t * ) addr );
				addr += 1;
			}
			else
			{
				byte_high = 0x00;
			}

            print_hex_value( byte_low );
			print_hex_value( byte_high );
		}

		uart_printstr( "\r\n" );
	}


	while ( 1 );

	return ( 0 );
}
