/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 11:32:35 by ltruchel          #+#    #+#             */
/*   Updated: 2022/12/05 13:00:19 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(int move)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (0);
	if (!move)
		new->move = 0;
	else
		new->move = move;
	new->next = NULL;
	return (new);
}
