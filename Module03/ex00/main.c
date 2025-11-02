/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:21:59 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/02 19:00:06 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://forum.arduino.cc/t/pulsing-rgb-led-understanding-ocm-and-phase-correct-pwm/1212393

#include <avr/io.h>
#include <util/delay.h>

// Broches de la LED RGB D5
#define RED_PIN   PD5  // Pin D5 (OC0B)
#define GREEN_PIN PD6  // Pin D6 (OC0A)
#define BLUE_PIN  PD7  // Pin D7

void init_rgb(void) {
    // Configure les broches en sortie
    DDRD |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);
    
    // Timer0 pour PWM sur Rouge (D5/OC0B) et Vert (D6/OC0A)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);  // Fast PWM
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // Non-inverting mode
    TCCR0B |= (1 << CS01) | (1 << CS00);     // Prescaler 64
}

void set_color(uint8_t red, uint8_t green, uint8_t blue) {
    // Rouge (PWM sur OC0B)
    OCR0B = red;
    
    // Vert (PWM sur OC0A)
    OCR0A = green;
    
    // Bleu (pas de PWM hardware sur D7, on/off seulement)
    if (blue > 0) {
        PORTD |= (1 << BLUE_PIN);
    } else {
        PORTD &= ~(1 << BLUE_PIN);
    }
}

int main(void) {
    init_rgb();
    
    while(1) {
        // Rouge
        set_color(255, 0, 0);
        _delay_ms(1000);
        
        // Vert
        set_color(0, 255, 0);
        _delay_ms(1000);
        
        // Bleu
        set_color(0, 0, 255);
        _delay_ms(1000);
    }
    
    return 0;
}