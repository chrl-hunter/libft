/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:30:37 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/20 18:46:32 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *lit, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (lit[0] == '\0')
		return ((char *)big);
	j = 0;
	while (big[i] && i < n)
	{
		while (i + j < n && big[i + j]
			&& big[i + j] == lit[j])
			j++;
		if (lit[j] == '\0')
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
