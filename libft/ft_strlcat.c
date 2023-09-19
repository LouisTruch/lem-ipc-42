/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 15:54:38 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/03 21:06:06 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_d;
	size_t	len_s;

	i = 0;
	len_d = 0;
	len_s = ft_strlen(src);
	if (size == 0)
		return (len_s);
	if (!dst && !src)
		return (0);
	while (dst[len_d] && len_d < size)
		len_d++;
	if (dst[len_d] != '\0')
		return (len_d + len_s);
	while (src[i] && len_d + i < size - 1)
	{
		dst[len_d + i] = src[i];
		i++;
	}
	dst[len_d + i] = '\0';
	return (len_d + len_s);
}
