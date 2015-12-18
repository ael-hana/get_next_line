/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 16:06:18 by ael-hana          #+#    #+#             */
/*   Updated: 2015/12/18 14:49:50 by ael-hana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list_fd					*ft_init_list(int fd, t_list_fd *next, int i)
{
	t_list_fd				*list;

	if (!(list = (t_list_fd *)malloc(sizeof(t_list_fd))))
		return (NULL);
	list->fd = fd;
	list->next = next;
	list->i = i + 1;
	list->content = ft_strdup("\0");
	return (list);
}

t_list_fd					*ft_search_fd(t_list_fd *list, int fd)
{
	void					*tmp;
	t_list_fd				*ltmp;

	if (!list)
		return (ft_init_list(fd, NULL, -1));
	while (list->i != 0)
		list = list->next;
	tmp = list;
	if (list->fd == fd)
		return (list);
	if (list->next)
		list = list->next;
	ltmp = list;
	while (list->i)
	{
		if (list->fd == fd)
			return (list);
		ltmp = list;
		list = list->next;
	}
	return (ltmp->next = ft_init_list(fd, tmp, ltmp->i));
}

char						check_buf(t_list_fd *list, char **line, char *buf)
{
	char					*ptr;
	void					*tmp;

	if ((ptr = ft_strchr(buf, '\n')))
	{
		*ptr = '\0';
		tmp = *line;
		*line = ft_strjoin(*line, buf);
		free(list->content);
		list->content = ft_strdup(++ptr);
		free(tmp);
		free(buf);
		return (1);
	}
	else if ((ptr = ft_strchr(list->content, '\n')))
	{
		*ptr = '\0';
		tmp = list->content;
		*line = ft_strdup(list->content);
		list->content = ft_strdup(++ptr);
		free(tmp);
		free(buf);
		return (1);
	}
	return (0);
}

int							get_next_line(int const fd, char **line)
{
	static t_list_fd		*list = NULL;
	char					*buf;
	int						len;
	char					*chr;

	if (fd < 0 || !line || !(list = ft_search_fd(list, fd)) || BUFF_SIZE <= 0)
		return (-1);
	if (!(buf = malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (-1);
	ft_bzero(buf, BUFF_SIZE + 1);
	if (check_buf(list, line, buf))
		return (1);
	*line = ft_strdup(list->content);
	while ((len = read(fd, buf, BUFF_SIZE)))
	{
		if (len == -1)
			return (-1);
		buf[len] = '\0';
		if (check_buf(list, line, buf))
			return (1);
		chr = *line;
		*line = ft_strjoin(*line, buf);
		free(chr);
	}
	return (0);
}
