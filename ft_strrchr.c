/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 20:52:35 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/20 18:30:06 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	tmp;

	i = 0;
	tmp = -1;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			tmp = i;
		i++;
	}
	if (tmp == -1)
		return (NULL);
	if ((unsigned char)c == '\0')
		return ((char *)&s[i]);
	return ((char *)&s[tmp]);
}
