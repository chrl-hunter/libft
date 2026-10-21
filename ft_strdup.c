/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 19:25:33 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/20 19:29:31 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*strdup(const char *s)
{
	void	*ptr;
	size_t	len;

	len = ft_strlen(s) + 1;
	ptr = malloc(len);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s, len);
	return (ptr);
}
