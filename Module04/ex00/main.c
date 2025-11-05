/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:19:58 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/04 23:13:06 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://wikigeii.iut-troyes.univ-reims.fr/index.php?title=Cours:Atmega328p

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DEBOUNCE_DELAY 20

volatile uint8_t button_state = 0;  // 0 = relâché, 1 = appuyé

ISR(INT0_vect)
{
    EIMSK &= ~(1 << INT0);  // Désactiver temporairement
    
    _delay_ms(30);  // Anti-rebond
    
    // Alterner entre front descendant et montant
    if (EICRA & (1 << ISC00))  // Si front montant (relâchement)
    {
        button_state = 0;  // Bouton relâché
        EICRA &= ~(1 << ISC00);  // Retour au front descendant
    }
    else  // Si front descendant (appui)
    {
        button_state = 1;  // Bouton appuyé
        EICRA |= (1 << ISC00);  // Passer au front montant
    }
    
    _delay_ms(20);
    EIMSK |= (1 << INT0);  // Réactiver
}

int main(void)
{
    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    
    cli();
    EICRA = (1 << ISC01);  // Front descendant
    EIMSK = (1 << INT0);
    sei();
    
    while (1)
    {
        if (button_state == 1)
        {
            PORTB |= (1 << PB0);   // LED allumée tant qu'appuyé
        }
        else
        {
            PORTB &= ~(1 << PB0);  // LED éteinte sinon
        }
    }
    
    return 0;
}
