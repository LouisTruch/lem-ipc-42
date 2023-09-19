/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 12:30:33 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/13 01:01:16 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	putchar_cnt(char c, int count)
{
	write(1, &c, 1);
	count++;
	return (count);
}

int	putstr_cnt(char *s, int count)
{
	if (!s)
	{
		count = putstr_cnt("(null)", count);
		return (count);
	}
	while (*s)
	{
		write(1, s, 1);
		s++;
		count++;
	}
	return (count);
}

int	basewrite_cnt(long long nb, char *base, int count)
{
	write (1, &base[nb], 1);
	count++;
	return (count);
}

int	putnbrbase_cnt(long long nb, char *base, int count)
{
	int	base_l;

	base_l = ft_strlen(base);
	if (nb < 0)
	{
		count = putchar_cnt('-', count);
		nb *= -1;
	}
	if (nb < base_l)
		count = basewrite_cnt(nb, base, count);
	else
	{
		count = putnbrbase_cnt(nb / base_l, base, count);
		count = basewrite_cnt(nb % base_l, base, count);
	}
	return (count);
}

int	putunbrbase_cnt(unsigned long long nb, char *base, int count)
{
	unsigned long long	base_l;

	base_l = ft_strlen(base);
	if (nb < base_l)
		count = basewrite_cnt(nb, base, count);
	else
	{
		count = putnbrbase_cnt(nb / base_l, base, count);
		count = basewrite_cnt(nb % base_l, base, count);
	}
	return (count);
}
