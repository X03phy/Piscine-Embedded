/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:08:26 by x03phy            #+#    #+#             */
/*   Updated: 2025/11/08 13:10:31 by x03phy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int ft_strcmp( const char *s1, const char *s2 )
{
	int i;

	if ( !s1 || !s2 )
		return ( 0 );

	i = 0;
	while ( s1[i] && s1[i] == s2[i] )
		i += 1;
	
	return ( s1[i] - s2[i] );
}

int ft_strlen( const char *s )
{
	int i;

	i = 0;
	while ( s[i] )
		i += 1;

	return ( i );
}
