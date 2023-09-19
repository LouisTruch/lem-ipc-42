/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 11:08:53 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/18 20:27:34 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*cst1;
	const unsigned char	*cst2;
	size_t				i;

	cst1 = (const unsigned char *)s1;
	cst2 = (const unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	if (cst1[i] == '\0' && cst2[i] == '\0')
		return (-1);
	while (cst1[i] == cst2[i] && n > i + 1)
		i++;
	return (cst1[i] - cst2[i]);
}
