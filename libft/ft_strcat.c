/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:46:41 by ltruchel          #+#    #+#             */
/*   Updated: 2022/11/08 19:53:49 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(const char	*s1, const char *s2)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new[i + j] = s2[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}
