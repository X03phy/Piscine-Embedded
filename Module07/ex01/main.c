/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:15:29 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/13 09:21:28 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include<avr/eeprom.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

/*  Utils  */
static int ft_toupper( int c )
{
	if ( c >= 'a' && c <= 'z' )
		return ( c - 32 );

	return ( c );
}

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
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ); // Enable transmission and reception
	UCSR0C = ( 1 << UCSZ00 ) | ( 1 << UCSZ01 ); // For character size (8 bits)
}

static void uart_tx( char c )
{
	// UDRE0 : USART Data Register Empty
	while ( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}

static char uart_rx( void )
{
	// RXC0 : Receive Complete
	while ( ! ( UCSR0A & ( 1 << RXC0 ) ) );

	return ( UDR0 );
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

static uint8_t hexchar_to_val( char c )
{
	if ( c >= '0' && c <= '9' ) return ( c - '0' );
	if ( c >= 'A' && c <= 'F' ) return ( c - 'A' + 10 );
}

uint16_t extract_addr( char *buffer )
{
	uint16_t addr = 0;

	for ( int i = 0; i < 8; i += 1 )
	{
		addr = ( addr << 4 ) | hexchar_to_val( buffer[i] );
	}

	return ( addr );
}

uint8_t extract_byte( char *buffer )
{
	uint8_t byte = 0;

	for ( int i = 9; i < 11; i += 1 )
	{
		byte = ( byte << 4 ) | hexchar_to_val( buffer[i] );
	}

	return ( byte );
}

void print_eeprom( uint16_t highlight_addr )
{
	int i;
	int j;
	uint16_t eaddr = 0;
	uint8_t byte_low, byte_high;
	uint16_t laddr, haddr;

	while ( eaddr <= E2END )
	{
		// address
		for ( i = 0; i < 7; i++ )
			uart_tx( "0123456789ABCDEF"[( eaddr >> ( 4 * ( 6 - i ) ) ) & 0x0F] );

		for ( j = 0; j < 8 && eaddr <= E2END; j++ )
		{
			uart_tx(' ');

			laddr = eaddr;
			byte_low = eeprom_read_byte( ( uint8_t * ) eaddr++ );
			haddr = eaddr;
			if ( eaddr <= E2END )
				byte_high = eeprom_read_byte( ( uint8_t * ) eaddr++ );
			else
				byte_high = 0x00;

			// byte_low
			if ( laddr == highlight_addr ) uart_printstr( "\033[31m" );
			print_hex_value( byte_low );
			if ( laddr == highlight_addr ) uart_printstr( "\033[0m" );

			// byte_high
			if ( haddr == highlight_addr ) uart_printstr( "\033[31m" );
			print_hex_value( byte_high );
			if ( haddr == highlight_addr ) uart_printstr( "\033[0m" );

		}
		uart_printstr( "\r\n" );
	}
}

int parse_input( char *buffer, uint16_t *addr, uint8_t *byte )
{
	// Parse buffer
	for ( int i = 0; i < 11; i++ )
	{
		if ( i == 8 )
		{
			if ( buffer[i] != ' ' )
				return ( 0 );
			continue;
		}
		if ( ! ( ( buffer[i] >= '0' && buffer[i] <= '9' ) ||
				( buffer[i] >= 'A' && buffer[i] <= 'F' ) ) )
			return ( 0 );
	}

	*addr = extract_addr( buffer );
	*byte = extract_byte( buffer );

	if ( *addr > E2END )
	{
		uart_printstr( "Address to big\r\n" );
		return ( 0 );
	}

	if ( eeprom_read_byte( ( uint8_t * ) *addr ) == *byte )
	{
		uart_printstr( "Same byte already here\r\n" );
		return ( 0 );
	}

	return ( 1 );
}

int main( void )
{
	uart_init();

	char c;
	char buffer[11]; // address (8) + space (1) + byte (2)
	int idx;

	uint16_t addr;
	uint8_t byte;

	uart_printstr( "Enter adress + byte : " );

	idx = 0;
	while ( 1 )
	{
		c = ft_toupper( uart_rx() );

		if ( c == 127 || c == 8 ) // Delete
		{
			if ( idx > 0 )
			{	
				idx -= 1;
				uart_printstr( "\b \b" );
			}
		}

		else if ( c == '\r' || c == '\n' ) // Enter
		{
			uart_printstr( "\r\n" );
			if ( idx != 11 )
			{
				uart_printstr( "Input incomplete !\r\n" );
			}

			else if ( parse_input( buffer, &addr, &byte ) )
			{
				eeprom_write_byte( ( uint8_t * ) addr, byte );

				print_eeprom( addr );
			}
			
			idx = 0;
			uart_printstr( "\r\nEnter adress + byte : " );
		}

		else
		{
			if ( idx < 11 )
			{
				uart_tx( c );
				buffer[idx++] = c;
			}
		}
	}

	return ( 0 );
}
