/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:55:28 by ltruchel          #+#    #+#             */
/*   Updated: 2022/09/28 18:30:54 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*casted;
	size_t	i;

	casted = (char *)s;
	i = 0;
	while (n > i)
	{
		casted[i] = c;
		i++;
	}
	return (s);
}
