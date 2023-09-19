/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 12:18:42 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/18 20:46:23 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	int		malloc_size;

	if (!s)
		return (NULL);
	malloc_size = ft_strlen(s);
	malloc_size -= start;
	if (malloc_size < 0)
		malloc_size = 0;
	if ((size_t)malloc_size > len)
		malloc_size = len;
	new = malloc(malloc_size + 1);
	if (!new)
		return (NULL);
	new[malloc_size] = '\0';
	malloc_size--;
	while (malloc_size >= 0)
	{
		new[malloc_size] = s[start + malloc_size];
		malloc_size--;
	}
	return (new);
}
