/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bda-silv <bda-silv@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:47:50 by bda-silv          #+#    #+#             */
/*   Updated: 2022/07/19 20:46:21 by bda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*read_buffer(int fd, char *buffer, ssize_t *flag)
{
	// flags: (-2) ERR alloc | ERR read (-1) | EOF (0) | BYTES READ (+)
	*flag = read(fd, buffer, BUFFER_SIZE);
	if (*flag < 0)
		return (NULL);
	buffer[*flag] = 0;
	return (buffer);
}

char	*rip_n_save(char *temp, char *cache)
{
	int		n;
	char	*line;
	char	*tmp;

	n = 0;
	if (!temp)
		return (NULL);
	tmp = ft_strdup(temp);
	n = (ft_strchr(tmp, '\n') - tmp) + 1;
	line = ft_substr(tmp, 0, n);
	free(cache);
	cache = ft_substr(tmp, n, ft_strlen(tmp) + 1);
	free(tmp);
	free(temp);
	return (line);
}

char	*build_line(int fd, ssize_t *flag)
{
	char	*buffer;
	char	*line;

	line = NULL;
	line = ft_strdup("\0");
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		*flag = -2;
		return (NULL);
	}
	while (*flag > 0 && !ft_strchr(line, '\n'))
	{
		buffer = read_buffer(fd, buffer, flag);
		line = ft_strjoin(line, buffer);
	}
	free(buffer);
	if (!*line)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*temp;
	char		*line;
	ssize_t		flag;

	temp = NULL;
	flag = 1;
	if (!cache)
		cache = ft_strdup("\0");
	if (BUFFER_SIZE < 1 || fd < 0 || fd > MAX_FD)
		return (NULL);
	if (!ft_strchr(cache, '\n'))
	{
		temp = build_line(fd, &flag);
		line = rip_n_save(temp, cache);
	}
	else
		line = rip_n_save(cache, cache);
	return (line);
}


int	main(void)//TODO: Implement argc, argv
{
	int		fd;
	char	*str;

	str = NULL;
	fd = open("t4.txt", O_RDONLY);
	do
	{
		str = get_next_line(fd);
		printf("%s", str);
		free(str);
	} while(str != NULL);
	return (0);
}

