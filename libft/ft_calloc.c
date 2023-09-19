/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 11:14:24 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/22 22:29:23 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*mem;
	size_t	i;
	size_t	l;

	i = 0;
	l = nmemb * size;
	if (size == 0 || nmemb == 0)
		return (malloc(0));
	if (size != 0 && l / size != nmemb)
		return (0);
	mem = malloc(l);
	if (!mem)
		return (0);
	while (i < l)
	{
		mem[i] = '\0';
		i++;
	}
	return (mem);
}
