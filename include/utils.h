/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebonutto <ebonutto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:27:04 by ebonutto          #+#    #+#             */
/*   Updated: 2025/11/07 12:42:43 by ebonutto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

/*──────────────────────────────────────────────*/
/*─ BITWISE MACROS ─────────────────────────────*/
/*──────────────────────────────────────────────*/

/* Set bit 'bit' in variable 'var' */
#define _SET( var, bit )      ( ( var ) |= ( 1UL << ( bit ) ) )

/* Clear bit 'bit' in variable 'var' */
#define _CLEAR( var, bit )    ( ( var ) &= ~ ( 1UL << ( bit ) ) )

/* Toggle bit 'bit' in variable 'var' */
#define _TOGGLE( var, bit )   ( ( var ) ^= ( 1UL << ( bit ) ) )

/* Read the state of bit 'bit' in variable 'var' (returns 0 or 1) */
#define _READ( var, bit )     ( ( ( var ) >> ( bit ) ) & 0x01 )

/* Write bit 'bit' in variable 'var' according to 'val' (0 or 1) */
#define _WRITE( var, bit, val ) ( ( val ) ? _SET( var, bit ) : _CLEAR( var, bit ) )






/* Basic functions */

#endif
