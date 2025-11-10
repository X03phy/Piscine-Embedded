/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 09:24:20 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/10 18:49:40 by x03phy           ###   ########.fr       */
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

void i2c_stop( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWSTO ) | ( 1 << TWEN );
}

static void i2c_write( unsigned char data )
{
	TWDR = data;
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN );
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
}

static uint8_t i2c_read_ack( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN ) | ( 1 << TWEA ); // Activate Read + ACK
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
	return ( TWDR );
}

static uint8_t i2c_read_nack( void )
{
	TWCR = ( 1 << TWINT ) | ( 1 << TWEN ); // Activate Read
	while ( ! ( TWCR & ( 1 << TWINT ) ) );
	return ( TWDR );
}

int main( void )
{
	int i;
	uint8_t data[7];
	uint32_t raw_humidity, raw_temperature;
	float humidity, temperature;
	float average[4]; // Humidity and Temperature
	char buffer[10];

	// Set to 0 for the first 2 measures.
	average[0] = '\0'; average[1] = '\0';
	average[2] = '\0'; average[3] = '\0';

	uart_init();
	i2c_init();

	uart_printstr( "Waiting...\r\n" );

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
		for ( i = 0; i < 6; i += 1 )
			data[i] = i2c_read_ack();
		data[6] = i2c_read_nack();
		i2c_stop();
		
		// Extract data
		raw_humidity = ( ( uint32_t ) ( data[1] ) << 12 ) |
					( ( uint32_t ) ( data[2] ) << 4 ) |
					( ( uint32_t ) ( data[3] >> 4 ) & 0x0F );
		humidity = ( ( float ) raw_humidity / 1048576.0f ) * 100.0f; // 2^20 = 1048576

		raw_temperature = ( ( ( uint32_t ) ( data[3] & 0x0F ) ) << 16 ) |
						( ( uint32_t ) ( data[4] ) << 8 ) |
						( ( uint32_t ) ( data[5] ) );
		temperature = ((float)raw_temperature / 1048576.0f) * 200.0f - 50.0f; // 2^20 = 1048576

		if ( average[0] != '\0' && average[1] != '\0' )
		{
			humidity = ( average[0] + average[1] + humidity ) / 3;
			temperature = ( average[2] + average[3] + temperature ) / 3;
			uart_printstr( "Temperature: " );
			uart_printstr( dtostrf( temperature, 2, 1, buffer ) );
			uart_printstr( " C, Humidity: " );
			uart_printstr( dtostrf( humidity, 4, 1, buffer ) );
			uart_printstr( " %\r\n" );			
		}

		average[0] = average[1];
		average[1] = humidity;

		average[2] = average[3];
		average[3] = temperature;

		_delay_ms( 2000 ); // It is recommended to measure data every 2 seconds
	}
	
	return ( 0 );
}
