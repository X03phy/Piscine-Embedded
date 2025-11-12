/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:15:29 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/13 00:05:34 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include<avr/eeprom.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

#define MAGIC_NUMBER_START E6
#define MAGIC_NUMBER_MID E7
#define MAGIC_NUMBER_END E8

#define MAGIC_NUMBER_START_VALUE 230
#define MAGIC_NUMBER_MID_VALUE 231
#define MAGIC_NUMBER_END_VALUE 232

/*  Utils  */
static int ft_strlen( const char *s )
{
	int i = 0;
	
	while ( s[i] )
		i++;
	
	return ( i );
}

static int ft_toupper( int c )
{
	if ( c >= 'a' && c <= 'z' )
		return ( c - 32 );

	return ( c );
}

static int ft_strncmp( const char *s1, const char *s2, int n )
{
	int i = 0;

	while ( s1[i] && i < n && s1[i] == s2[i] )
		i++;

	if ( i == n )
		return 0;

	return ( s1[i] - s2[i] );
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


/*  EEPROM  */
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

static void get_line( char *buffer )
{
	char c;
	int idx = 0;

	uart_tx( '>' );

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

		else if ( c == '\r' || c == '\n' ) // Enter
		{
			buffer[idx] = '\0';
			break;
		}

		else
		{
			if ( idx < 42  )
			{
				uart_tx( c );
				buffer[idx++] = c;
			}
		}
	}
}

static int parse_key( char *buffer, char *key )
{
	int i;
	int j;

	i = 0;
	while ( buffer[i] != ' ' ) // We already know it has a space
		i++;

	i++; // Skip  space

	if ( buffer[i] != '\"' )
	{
		uart_printstr( "\r\nInvalid Quotes!\r\n" );
		return ( 0 );
	}
	
	i += 1; // Skip quote

	while ( buffer[i + j] != '\"' )
	{
		if ( buffer[i + j] == '\0' )
		{
			uart_printstr( "\r\nInvalid Quotes!\r\n" );
			return ( 0 );
		}

		if ( j > 32 )
		{
			uart_printstr( "\r\nKey too long!\r\n" );
			return ( 0 );
		}

		key[j] = buffer[i + j];

		j += 1;
	}

	key[j] = '\0';

	return ( 1 );
}

static int parse_value( char *buffer, char *value )
{
	int i;
	int j;

	i = 0;
	while ( buffer[i] != ' ' ) // We already know it has a space
		i++;

	i += 2; // Skip  space and quote
	
	while ( buffer[i] != '\"' )
		i++;

	i += 1; // Skip quote

	if ( buffer[i] != ' ' )
	{
		uart_printstr( "\r\nInvalid value!\r\n" );
		return ( 0 );	
	}

	i++; // Skip space

	if ( buffer[i] != '\"' )
	{
		uart_printstr( "\r\nInvalid Quotes!\r\n" );
		return ( 0 );
	}

	i += 1; // Skip quote

	j = 0;
	while ( buffer[i + j] != '\"' )
	{
		if ( buffer[i + j] == '\0' )
		{
			uart_printstr( "\r\nInvalid Quotes!\r\n" );
			return ( 0 );
		}

		if ( j > 32 )
		{
			uart_printstr( "\r\nValue too long!\r\n" );
			return ( 0 );
		}

		value[j] = buffer[i + j];

		j += 1;
	}

	value[j] = '\0';

	return ( 1 );
}

static void handle_read( char *buffer )
{
	uint16_t eaddr = 0;
	uint8_t c;
	uint8_t key[33];
	uint8_t stored_key[33];
	int i = 0;

	if ( parse_key( buffer, key ) == 0 )
	{
		return ;
	}

	while ( eaddr <= E2END )
	{
		// Starting point
		if ( eeprom_read_byte( ( uint8_t * ) eaddr ) == ( uint8_t ) MAGIC_NUMBER_START_VALUE )
		{
			eaddr++; // Skip MAGIC_NUMBER_START_VALUE

			while ( ( c = eeprom_read_byte( ( uint8_t * ) eaddr ) ) != MAGIC_NUMBER_MID_VALUE && i < 32 )
			{
				stored_key[i] = c;
				eaddr += 1;
				i += 1;
			}
			stored_key[i] = '\0';
			// uart_printstr( "\r\nstored_key: " );
			// uart_printstr( stored_key );
			// uart_printstr( "\r\n" );


			if ( ft_strncmp( stored_key, key, 32 ) == 0 )
			{
				while ( eeprom_read_byte( ( uint8_t * ) eaddr ) != MAGIC_NUMBER_MID_VALUE )
					eaddr++;

				eaddr++; // For MAGIC_NUMBER_MID

				uart_printstr( "\r\n" );

				while ( ( c = eeprom_read_byte( ( uint8_t * ) eaddr ) ) != MAGIC_NUMBER_END_VALUE )
				{
					uart_tx( c );
					eaddr++;
				}

				uart_printstr( "\r\n" );

				return ;
			}
		}

		eaddr += 1;
	}

	uart_printstr( "\r\nempty\r\n" );
}

static void handle_write( char *buffer )
{
	uint16_t eaddr = 0;
	unsigned char key[33];
	unsigned char value[33];

	if ( !parse_key( buffer, key ) || !parse_value( buffer, value ) )
	{
		return ;
	}

	if ( ft_strlen( key ) + ft_strlen( value ) > 32 )
	{
		uart_printstr( "\r\nsize of key + value too long\r\n" );
		return ;
	}

	eeprom_write_byte( ( uint8_t * ) eaddr, MAGIC_NUMBER_START_VALUE );
	eaddr += 1;

	for ( int i = 0; key[i]; i++ )
	{
		eeprom_write_byte( ( uint8_t * ) eaddr, key[i] );
		eaddr += 1;
	}
	eeprom_write_byte( ( uint8_t * ) eaddr, MAGIC_NUMBER_MID_VALUE );
	eaddr += 1;

	for ( int i = 0; value[i]; i++ )
	{
		eeprom_write_byte( ( uint8_t * ) eaddr, value[i] );
		eaddr += 1;
	}
	eeprom_write_byte( ( uint8_t * ) eaddr, MAGIC_NUMBER_END_VALUE );

	uart_printstr( "\r\n" );
}

static void print_eeprom( void )
{
	int i;
	uint16_t eaddr = 0;
	uint8_t byte_low, byte_high;
	uint16_t laddr, haddr;
	uint8_t buf[17];
	uart_printstr( "\r\n" );

	while ( eaddr <= 100 )
	{
		// address
		for ( i = 0; i < 7; i++ )
			uart_tx( "0123456789ABCDEF"[( eaddr >> ( 4 * ( 6 - i ) ) ) & 0x0F] );

		for ( i = 0; i < 16 && eaddr <= E2END; i += 2 )
		{
			uart_tx(' ');

			laddr = eaddr;
			byte_low = eeprom_read_byte( ( uint8_t * ) eaddr++ );
			haddr = eaddr;
			if ( eaddr <= E2END )
				byte_high = eeprom_read_byte( ( uint8_t * ) eaddr++ );
			else
				byte_high = 0x00;

			print_hex_value( byte_low );
			print_hex_value( byte_high );

			buf[i] = byte_low;
			buf[i + 1] = byte_high;
		}

		buf[16] = '\0';

		uart_tx( ' ' );
		uart_tx( '|' );
		for ( i = 0; buf[i]; i++ )
		{
			if ( 32 <= buf[i] && buf[i] <= 126 )
				uart_tx( buf[i] );
			else
				uart_tx( ' ' );
		}

		uart_tx( '|' );
		uart_printstr( "\r\n" );
	}
}

int main( void )
{
	uart_init();

	char buffer[42];

	uint16_t addr;
	uint8_t byte;

	while ( 1 )
	{
		get_line( buffer );

		if ( ft_strncmp( "READ ", buffer, 5 ) == 0 )
			handle_read( buffer );
		else if ( ft_strncmp( "WRITE ", buffer, 6 ) == 0 )
			handle_write( buffer );
		else if ( ft_strncmp( "PRINT", buffer, 6 ) == 0 )
			print_eeprom();
		else
			uart_printstr( "\r\nInvalid Command!\r\n" );
	}

	return ( 0 );
}
