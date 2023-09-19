/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 19:38:06 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/03 18:20:58 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void	*dest, const void *src, size_t n)
{
	char	*cstsrc;
	char	*cstdest;
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (dest);
	cstsrc = (char *)src;
	cstdest = (char *)dest;
	while (n > i)
	{
		cstdest[i] = cstsrc[i];
		i++;
	}
	return (dest);
}
