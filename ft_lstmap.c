/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cperez-h <cperez-h@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by cperez-h          #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by cperez-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*clear_map(t_list **new, void *content, void (*del)(void *))
{
	if (del)
		del(content);
	ft_lstclear(new, del);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*node;
	t_list	*last;
	void	*content;

	new = NULL;
	last = NULL;
	if (!lst || !f)
		return (NULL);
	while (lst)
	{
		content = f(lst->content);
		node = ft_lstnew(content);
		if (!node)
			return (clear_map(&new, content, del));
		if (!new)
			new = node;
		else
			last->next = node;
		last = node;
		lst = lst->next;
	}
	return (new);
}
