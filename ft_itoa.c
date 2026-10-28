/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 20:27:54 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/26 23:52:46 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_num(unsigned int n)
{
	size_t	i;

	i = 1;
	while (n >= 10)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char			*result;
	unsigned int	nb;
	size_t			len;

	nb = n;
	if (n < 0)
		nb = 0U - nb;
	len = count_num(nb) + (n < 0);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	while (nb >= 10)
	{
		result[--len] = '0' + nb % 10;
		nb /= 10;
	}
	result[--len] = '0' + nb;
	if (n < 0)
		result[0] = '-';
	return (result);
}
