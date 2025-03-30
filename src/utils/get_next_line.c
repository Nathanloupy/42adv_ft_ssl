#include "commons.h"

void	*ft_movebuf_left_clear(char *buf, size_t start, size_t imax)
{
	size_t	i;

	if (imax <= 0 || !buf)
		return (buf);
	i = 0;
	while (start + i <= imax)
	{
		buf[i] = buf[start + i];
		i++;
	}
	i = imax - start;
	while (i <= imax)
	{
		buf[i] = '\0';
		i++;
	}
	return (buf);
}

char	*ft_add_line(char *line, char *buf, size_t n)
{
	char	*temp;

	temp = ft_strnjoin_gnl(line, buf, n);
	free(line);
	return (temp);
}

char	*get_next_line_checked(int fd, ssize_t e, char *line)
{
	static char	buf[1024][BUFFER_SIZE + 1];

	if (fd < 0 || fd > 1023)
		return (NULL);
	while (e > 0)
	{
		e = get_endli(buf[fd]);
		if (e == -1)
			line = ft_add_line(line, buf[fd], BUFFER_SIZE + 1);
		else if (e >= 0)
		{
			line = ft_add_line(line, buf[fd], e + 1);
			if (e < BUFFER_SIZE)
				ft_movebuf_left_clear(buf[fd], e + 1, BUFFER_SIZE);
			else
				bzero(buf[fd], BUFFER_SIZE + 1);
			return (line);
		}
		e = read(fd, buf[fd], BUFFER_SIZE);
		if (e < 0)
			return (free(line), bzero(buf[fd], BUFFER_SIZE + 1), NULL);
		buf[fd][e] = '\0';
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char	*line;
	ssize_t	e;

	line = NULL;
	e = 1;
	if (BUFFER_SIZE <= 0)
		return (NULL);
	return (get_next_line_checked(fd, e, line));
}