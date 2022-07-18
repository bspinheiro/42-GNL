/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bda-silv <bda-silv@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:47:50 by bda-silv          #+#    #+#             */
/*   Updated: 2022/07/18 01:17:44 by bda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

void *ft_calloc(size_t size)
{
	size_t *ptr;
	size_t i;

	i = 0;
	ptr = malloc(sizeof(char) * (size + 1));
	if (!ptr)
		return (NULL);
	else
		while(i < size + 1)
		{
			ptr[i] = 0;
			i++;
		}
	return (ptr);
}

char	*read_buffer(int fd, char *buffer, ssize_t *flag)
{
	// flags: (-2) ERR alloc | ERR read (-1) | EOF (0) | BYTES READ (+)
	*flag = read(fd, buffer, BUFFER_SIZE);
	if (*flag < 0)
		return (NULL);
	buffer[*flag] = 0;
	return (buffer);
}

char	*sort_n_save_cache(char *tmp, char *cache)
{
	char *line;
	int n;

	line = NULL;
	cache = NULL;
	n = (ft_strchr(tmp, '\n') - tmp) + 1;
	line = ft_calloc(ft_strlen(tmp));
	cache = ft_calloc(ft_strlen(tmp));
	//proteger line e cache contra vazios ?
	ft_memmove(line, tmp, (ft_strchr(tmp, '\n') - tmp) + 1);
	ft_memmove(cache, tmp + n, ft_strlen(tmp) - n);
	free(tmp);
	return (line);
}

char	*build_line(int fd, ssize_t *flag)
{
	static char		*cache;
	char			*buffer;
	char			*line;
	char			*tmp;

	line = NULL;
	tmp = NULL;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		*flag = -2;
		return (NULL);
	}
	while (*flag > 0)
	{
		buffer = read_buffer(fd, buffer, flag);

		if (ft_strlen(cache) != 0)
		{
			tmp = ft_strjoin(cache, tmp);
			free(cache);
		}
		tmp = ft_strjoin(tmp, buffer);
		if (ft_strchr(tmp, '\n'))
			break;
	}
	line = sort_n_save_cache(tmp, cache);
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	ssize_t		flag;

	line = NULL;
	flag = 1;
	if (BUFFER_SIZE < 1 || fd < 0 || fd > MAX_FD)
		return (NULL);
	line = build_line(fd, &flag);
	if (flag < 0)
	{
		free(line);
		return (NULL);
	}
	return(line);
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
