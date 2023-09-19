/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 10:52:09 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/07 14:09:08 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*cstsrc;
	char	*cstdest;
	size_t	i;

	if (!dest && !src)
		return ((void *)0);
	i = 0;
	cstsrc = (char *)src;
	cstdest = (char *)dest;
	if (dest > src)
	{
		while (n-- > 0)
			cstdest[n] = cstsrc[n];
	}
	else
	{
		while (n > i)
		{
			cstdest[i] = cstsrc[i];
			i++;
		}
	}
	return ((void *)dest);
}
