/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:02:03 by cperez-h          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/10/15 13:55:09 by cperez-h         ###   ########.fr       */
=======
/*   Updated: 2026/10/14 13:17:49 by cperez-h         ###   ########.fr       */
>>>>>>> refs/remotes/origin/master
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char		*s;
	unsigned char			*d;
	size_t					i;

	if (!dest && !src)
		return (NULL);
<<<<<<< HEAD
	if (dest == src || !n)
		return (dest);
=======
>>>>>>> refs/remotes/origin/master
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	if (dest == src || !n)
		return (dest);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
