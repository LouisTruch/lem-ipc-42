/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:30:59 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/07 12:21:28 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	buffer;

	i = 0;
	buffer = 0;
	while (c > 255)
		c -= 256;
	while (s[i])
	{
		if (s[i] == c)
			buffer = i;
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	if (buffer == 0 && s[buffer] != c)
		return (0);
	return ((char *)&s[buffer]);
}
