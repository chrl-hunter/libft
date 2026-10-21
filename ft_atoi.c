/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:58:04 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/20 19:17:23 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_atoi(const char *ptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			sign = -1;
		i++;
	}
	while (ptr[i] >= '0' && ptr[i] <= '9')
	{
		result = result * 10 + (ptr[i] - '0');
		i++;
	}
	return (result * sign);
}
