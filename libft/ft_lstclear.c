/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 14:33:00 by ltruchel          #+#    #+#             */
/*   Updated: 2022/12/05 17:01:24 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst)
{
	t_list	*current;
	t_list	*temp;

	if (!lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	*lst = NULL;
}
