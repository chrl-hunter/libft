/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:08:19 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/20 18:23:37 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s1)[i]
				!= ((const unsigned char *)s2)[i])
			return (((const unsigned char *)s1)[i]
					-((const unsigned char *)s2)[i]);
		i++;
	}
	return (NULL);
}
