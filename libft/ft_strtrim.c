/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 13:16:32 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/18 20:32:34 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include<stdio.h>

static int	ft_inset(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_sizenew(char const *s1, char const *set)
{
	int	i;
	int	end;
	int	size;

	i = 0;
	end = ft_strlen(s1) - 1;
	size = end;
	while (ft_inset(set, s1[i]) == 1)
	{
		size -= 1;
		i++;
	}
	while (ft_inset(set, s1[end]) == 1)
	{
		size -= 1;
		end--;
	}
	size++;
	if (size < 0)
		return (0);
	return (size);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	int		i;
	int		j;
	int		end;

	i = 0;
	j = 0;
	if (!s1)
		return (0);
	end = ft_strlen(s1) - 1;
	new = malloc((sizeof(char) * (ft_sizenew(s1, set) + 1)));
	if (!new)
		return (0);
	while (ft_inset(set, s1[i]) == 1)
		i++;
	while (ft_inset(set, s1[end]) == 1)
		end--;
	while (i <= end)
	{
		new[j] = s1[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}
