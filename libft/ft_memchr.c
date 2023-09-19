/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 11:01:26 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/07 14:24:09 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*casted;
	size_t	i;

	casted = (char *)s;
	i = 0;
	if (n == 0)
		return (0);
	if (c > 255)
		c -= 256;
	while (n > i)
	{
		if (casted[i] == c)
			return ((char *)s + i);
		i++;
	}
	return (0);
}
