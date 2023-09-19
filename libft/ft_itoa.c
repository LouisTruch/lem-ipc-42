/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 16:47:24 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/18 20:20:28 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intsize(long long int n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n > 9)
	{
		n = n / 10;
		size++;
	}
	size++;
	return (size);
}

char	*ft_itoa(int n)
{
	long long int	lln;
	char			*arr;
	int				size;

	lln = n;
	size = ft_intsize(lln);
	arr = malloc(sizeof(char) * (size + 1));
	if (!arr)
		return (0);
	arr[size] = '\0';
	size--;
	if (lln < 0)
	{
		arr[0] = '-';
		lln = -lln;
	}
	while (lln > 9)
	{
		arr[size] = lln % 10 + '0';
		lln = lln / 10;
		size--;
	}
	arr[size] = lln % 10 + '0';
	return (arr);
}
