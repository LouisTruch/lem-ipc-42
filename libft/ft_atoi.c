/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:07:27 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/18 20:33:02 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_overflow(long rslt)
{
	if (rslt > INT_MAX)
		return (-1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	long	rslt;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	rslt = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		rslt = rslt * 10 + str[i] - '0';
		if (rslt * sign > INT_MAX || rslt * sign < INT_MIN)
			return (ft_overflow(rslt * sign));
		i++;
	}	
	return (rslt * sign);
}
