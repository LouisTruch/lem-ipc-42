/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 19:27:06 by ltruchel          #+#    #+#             */
/*   Updated: 2022/09/29 16:59:00 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*casted;
	size_t	i;

	casted = (char *)s;
	i = 0;
	while (n > i)
	{
		casted[i] = '\0';
		i++;
	}
	return ;
}
