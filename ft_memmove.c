/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:13:02 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/15 17:32:49 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h" 

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (dest == src || !n)
		return (dest);
	i = 0;
	while (i < n && (const unsigned char *)src + i != dest)
		i++;
	if (i == n)
		return (ft_memcpy(dest, src, n));
	while (n > 0)
	{
		n--;
		((unsigned char *)dest)[n] = ((const unsigned char *)src)[n];
	}
	return (dest);
}
