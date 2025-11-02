/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:48:56 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/02 18:19:17 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://marcjuneau.ca/?p=129

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE ( ( F_CPU / 8 / BAUD ) - 1 )

typedef enum {
	STATE_USERNAME,
	STATE_PASSWORD,
	STATE_DONE
} t_state;

const char *g_username = "ebonutto";
const char *g_password = "4242";

int g_state = STATE_USERNAME;
int g_pos = 0;
char g_input_username[256];
char g_input_password[256];

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

static void uart_init( void )
{	
	// Configure USART Baud Rate Registers
	UBRR0H = BAUD_PRESCALE >> 8; 
	UBRR0L = BAUD_PRESCALE;

	// Configure USART Control and Status Register
	// TXEN0 : Transmitter Enable bit
	// RXEN0 : Receiver Enable bit
	UCSR0A = ( 1 << U2X0 ); // Double Transmission Speed
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ) | ( 1 << RXCIE0 ); // TX, RX and RX interrupt enable
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

static void led_init( void )
{
	DDRB |= ( 1 << PB1 );
}

static void led_blink(void)
{
	for ( uint8_t i = 0; i < 10; i += 1 )
	{
		PORTB ^= ( 1 << PB1 );
		_delay_ms( 300 );
	}
}

ISR( USART_RX_vect )
{
	const char c = UDR0;

	if ( g_state == STATE_DONE )
		return ;

	if ( c == 127 || c == 8 ) // Delete
	{
		if ( g_pos > 0 )
		{	
			g_pos -= 1;
			uart_printstr( "\b \b" );
		}
		return ;
	}

	if ( c == '\r' || c == '\n' ) // Enter
	{
		uart_printstr( "\r\n" );
		if ( g_state == STATE_USERNAME )
		{
			g_input_username[g_pos] = '\0';
			g_pos = 0;
			g_state = STATE_PASSWORD;
			uart_printstr( "\tpassword: " );
		}
		else if ( g_state == STATE_PASSWORD )
		{
			g_input_password[g_pos] = '\0';
			g_pos = 0;
			
			if ( !ft_strcmp( g_input_username, g_username ) && !ft_strcmp( g_input_password, g_password ) )
			{
				uart_printstr( "Verifying" );
				for ( uint8_t i = 0; i < 3; i += 1 )
				{
					_delay_ms( 400 );
					uart_tx( '.' );
				}
				uart_printstr("\r\n");
				uart_printstr( "Hello " );
				uart_printstr( g_input_username );
				uart_printstr( "!\r\n" );
				led_blink();
				g_state = STATE_DONE;
			}
			else
			{
				uart_printstr( "Bad combinaison username/password\r\n" );
				uart_printstr("\r\nEnter your login:\r\n\tusername: ");
				g_state = STATE_USERNAME;
			}
		}
		return ;
	}

	if ( g_pos < 255 )
	{
		if ( g_state == STATE_USERNAME )
		{
			g_input_username[g_pos++] = c;
			uart_tx( c );
		}
		else if ( g_state == STATE_PASSWORD )
		{
			g_input_password[g_pos++] = c;
			uart_tx( '*' );
		}
	}
}

int main( void )
{
	uart_init();
	led_init();
	sei();

	uart_printstr("Enter your login:\r\n\tusername: ");

	while ( 1 );

	return ( 0 );
}
