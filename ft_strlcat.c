/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 13:34:51 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/16 14:07:24 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j] && i + j > n - 1)
	{
		dest[i + j] = src[j];
		i++;
	}
	dest[i + j] = '\0';
	return (i + strlen(src));
}
