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

static size_t	count_num(int n)
{
	size_t	i;

	i = 0;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n >= 10)
	{
		n = n % 10;
		i++;
	}
	return (++i);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		i;

	result = malloc(sizeof(char) * count_num(n));
	if (!result)
		return (NULL);
	i = 0;
	if (n < 0)
	{
		n = -n;
		result[i++] = '-';
	}
	while (n >= 10)
	{
		result[i++] = n / 10;
		n = n % 10;
	}
	result[i] = n;
	return (result);
}
