/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:39:13 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/13 09:55:59 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

// 19 SPI – Serial Peripheral Interface

void spi_init( void )
{
	// PB2 -> SS (Slave Select)
	// PB3 -> MOSI (Master Out Slave In)
	// PB5 -> SCK (Serial Clock)

	// Table 19-1.
	DDRB |= ( 1 << PB2 ) | ( 1 << PB3 ) | ( 1 << PB5 ); // SS, MOSI, SCK as output


	// SPE -> Activate SPI module
	// MSTR -> Set as master
	// SPR -> Frequency
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 );
}

// Page 172
void spi_send( uint8_t data )
{
	/* Start transmission */
    SPDR = data;
	/* Wait for transmission complete */
    while ( ! ( SPSR & ( 1 << SPIF ) ) );
}

int main( void )
{
	spi_init();
	spi_set_led_color( led, brightness, 0XFF0000 );
	spi_close
	while ( 1 );
}
