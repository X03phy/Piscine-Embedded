/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:27:04 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/08 14:22:11 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

/*──────────────────────────────────────────────*/
/*─ SIZE ───────────────────────────────────────*/
/*──────────────────────────────────────────────*/

#define _SIZE( tab ) ( sizeof( tab ) / sizeof( tab[0] ) )

/*──────────────────────────────────────────────*/
/*─ BIT MANIPULATION ───────────────────────────*/
/*──────────────────────────────────────────────*/

/* Create bit mask from bit number */
#define _BIT_VALUE( bit ) ( 1 << ( bit ) )

/*──────────────────────────────────────────────*/
/*─ BITWISE MACROS ─────────────────────────────*/
/*──────────────────────────────────────────────*/

/* Set bit 'bit' in variable 'var' */
#define _SET( var, bit )        ( ( var ) |= ( 1UL << ( bit ) ) )

/* Clear bit 'bit' in variable 'var' */
#define _CLEAR( var, bit )      ( ( var ) &= ~ ( 1UL << ( bit ) ) )

/* Toggle bit 'bit' in variable 'var' */
#define _TOGGLE( var, bit )     ( ( var ) ^= ( 1UL << ( bit ) ) )

/* Read the state of bit 'bit' in variable 'var' (returns 0 or 1) */
#define _READ( var, bit )       ( ( ( var ) >> ( bit ) ) & 0x01 )

/* Write bit 'bit' in variable 'var' according to 'val' (0 or 1) */
#define _WRITE( var, bit, val ) ( ( val ) ? _SET( var, bit ) : _CLEAR( var, bit ) )

/* ────────────────────────────────────────────── */
/* ─ BASIC FUNCTIONS ──────────────────────────── */
/* ────────────────────────────────────────────── */

int ft_strcmp( const char *s1, const char *s2 );
int ft_strlen( const char *s );

#endif
