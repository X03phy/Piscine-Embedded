/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 09:24:20 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/12 12:17:37 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://rex.plil.fr/Enseignement/Systeme/Systeme.M2-I2L/systeme013.html

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define SCL_CLOCK 100000UL  // 100 kHz

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

#define AHT20_ADDR 0x38 // AHT20 sensor adress
#define I2C_WRITE 0     // Where to write
#define I2C_READ 1     // Where to read

volatile uint8_t data = 0xAC;  // Test
volatile uint8_t i2c_status;   // Store I2C status

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

/*  I2C  */
static void i2c_init( void )
{
	TWSR = 0;                                // Default clock
	TWBR=( ( F_CPU / SCL_CLOCK ) - 16 ) / 2; // I2C clock frequency
	TWCR = ( 1 << TWEN );                    // Material I2C activation
}

// Last transmission state
static uint8_t i2c_statut( void )
{
	return ( TWSR & 0xF8 );
}

static void i2c_start( void )
{
	// Start
	TWCR = ( 1 << TWINT ) | ( 1 << TWSTA ) | ( 1 << TWEN );
	while( ( TWCR & ( 1 << TWINT ) ) == 0 );  // wating till end of send
}

static void i2c_stop( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWSTO ) | ( 1 << TWEN );
	while ( TWCR & ( 1 << TWSTO ) );
}

static void i2c_write( unsigned char data )
{
	TWDR = data;
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN );
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
}

static void i2c_read( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN ) | ( 1 << TWEA ); // Activate Read + ACK
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
	print_hex_value( TWDR );
}

static void i2c_read_nack( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN ); // Activate Read
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
	print_hex_value( TWDR );
}

int main( void )
{
	uart_init();
	i2c_init();

	while ( 1 )
	{
		i2c_start();
		i2c_write( ( AHT20_ADDR << 1 ) | I2C_WRITE );
		i2c_write( 0xAC ); // Measurement command
		i2c_write( 0x33 ); // Measure parameter
		i2c_write( 0x00 ); // Padding
		i2c_stop();

		_delay_ms( 80 );
		
		// read temperature and humidity data
		i2c_start();
		i2c_write( ( AHT20_ADDR << 1 ) | I2C_READ );
		i2c_read(); // state
		uart_tx( ' ' );
		i2c_read(); // humidity
		uart_tx( ' ' );
		i2c_read(); // humidity
		uart_tx( ' ' );
		i2c_read(); // humidity and temperature
		uart_tx( ' ' );
		i2c_read(); // temperature
		uart_tx( ' ' );
		i2c_read(); // temperature
		uart_tx( ' ' );
		i2c_read_nack(); // CRC data
		i2c_stop();

		uart_printstr( "\r\n" );

		_delay_ms( 2000 ); // It is recommended to measure data every 2 seconds
	}
	
	return ( 0 );
}
