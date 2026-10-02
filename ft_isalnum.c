/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 13:19:38 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/02 13:22:31 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_isalpha.c"
#include "ft_isdigit.c"

int	ft_isalnum(int	c)
{
	if  (ft_isalpha(c) == 1 || ft_isdigit(c) == 1)
		return (1);
	return (0);
}
