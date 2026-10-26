/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 06:23:51 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/22 06:46:26 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	size_t	size;

	if (!s1 || !set)
		return (NULL);
	size = ft_strlen(s1) - ft_strlen(set);
	ptr = malloc(sizeof(char) * (size + 1));
	if (!ptr)
		return (NULL);
	while (*s1)
	{
		if (*s1 != *set)
		{
			*ptr = *s1;
			s1++;
			ptr++;
		}
		else if (*s1 == *set)
			s1++;
	}
	return (ptr);
}
