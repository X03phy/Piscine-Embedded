/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 19:25:45 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/08 20:51:19 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/io.h>

/* Macros */

/*  Doc  */
#define ENABLE_INT0()  ( EIMSK |= ( 1 << INT0 ) )
#define DISABLE_INT0() ( EIMSK &= ~ ( 1 << INT0 ) )

#define SET_INT0_FALLING_EDGE() ( EICRA = ( EICRA & ~ ( 1 << ISC00 ) ) | ( 1 << ISC01 ) )
#define SET_INT0_RISING_EDGE()  ( EICRA |= ( ( 1 << ISC01 ) | ( 1 << ISC00 ) ) )

#define IS_INT0_FALLING_EDGE()  ( ( EICRA & ( 1 << ISC01 ) ) && ! ( EICRA & ( 1 << ISC00 ) ) )
#define IS_INT0_RISING_EDGE()   ( ( EICRA & ( 1 << ISC01 ) ) && ( EICRA & ( 1 << ISC00 ) ) )

#endif
