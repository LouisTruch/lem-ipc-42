/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 16:47:02 by ltruchel          #+#    #+#             */
/*   Updated: 2022/10/24 14:29:54 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Free all the tabs + the whole tab if something fails                    */
static void	ft_free(char **tab, int k)
{
	int	i;

	i = 0;
	while (i < k)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return ;
}

/* Count number of word in a given string                                  */
static int	ft_cntword(char const *s, char c)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	if (s[i] == 0)
		return (0);
	if (s[i] != c)
		cnt++;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c
			&& s[i + 1] != '\0')
				cnt++;
		i++;
	}
	return (cnt);
}

/* Find and return the length of a word from sep to sep or \0              */
static int	ft_wordlength(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

/* Find and return the index of the next char that isn't a sep              */

static int	ft_findnextword(char const *s, char c, int i)
{
	while (s[i] == c && s[i])
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;
	int		l;
	int		k;

	i = 0;
	l = 0;
	k = -1;
	if (!s)
		return (0);
	tab = malloc(sizeof(char *) * (ft_cntword(s, c) + 1));
	if (!tab)
		return (0);
	while (++k < ft_cntword(s, c))
	{
		i = ft_findnextword(s, c, i + l);
		l = ft_wordlength(s + i, c);
		tab[k] = ft_substr(s, i, l);
		if (!tab[k])
			ft_free(tab, k);
		if (!tab[k])
			return (NULL);
	}
	tab[k] = NULL;
	return (tab);
}
