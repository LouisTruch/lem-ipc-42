/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltruchel <ltruchel@student.42angoulem      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:14:50 by ltruchel          #+#    #+#             */
/*   Updated: 2022/12/08 11:22:54 by ltruchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Join stash and buf using a tmp to avoid leaks             */

char	*ft_join(char *stash, char *buf)
{
	char	*tmp;

	tmp = ft_strjoin(stash, buf);
	free (buf);
	free (stash);
	stash = ft_strdup(tmp);
	free (tmp);
	return (stash);
}

/* Trim line before first \n from **stash 
 * and adjust **stash after first \n                         */

char	*ft_trim(char **stash)
{
	char	*line;
	char	*tmp;
	int		i;

	i = 0;
	if (!(*stash))
		return (NULL);
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	line = ft_substr(*stash, 0, i + 1);
	tmp = ft_strdup(*stash);
	free (*stash);
	*stash = ft_substr(tmp, i + 1, ft_strlen(tmp));
	free (tmp);
	return (line);
}

/* Read file, join until /n is found or eof                    */

char	*ft_read_file(int fd, char *stash)
{
	char	*buf;
	int		byte_r;

	byte_r = 1;
	while (byte_r > 0)
	{
		buf = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
		if (!buf)
			return (NULL);
		byte_r = read(fd, buf, BUFFER_SIZE);
		if (byte_r <= 0)
		{
			free (buf);
			if (stash && !stash[0])
			{
				free (stash);
				stash = NULL;
			}
			return (stash);
		}
		stash = ft_join(stash, buf);
		if (ft_strchr(stash, '\n') != 0)
			break ;
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*line;

	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash[fd])
	{
		stash[fd] = ft_calloc(1, 1);
		if (!stash[fd])
			return (NULL);
	}
	if (ft_strchr(stash[fd], '\n') == 0)
		stash[fd] = ft_read_file(fd, stash[fd]);
	line = ft_trim(&stash[fd]);
	if (stash[fd] && !stash[fd][0])
	{
		free (stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
