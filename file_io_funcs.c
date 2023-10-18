#include "shell.h"

/**
 * g_get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *g_get_history_file(infoGodzilla_t *info)
{
	char *buf, *dir;

	dir = g_getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (g_strlen(dir) + g_strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	g_strcpy(buf, dir);
	g_strcat(buf, "/");
	g_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * g_write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int g_write_history(infoGodzilla_t *info)
{
	ssize_t fd;
	char *filename = g_get_history_file(info);
	listGodzilla_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		g_putsfd(node->str, fd);
		g_putfd('\n', fd);
	}
	g_putfd(BUFFER_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * g_read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int g_read_history(infoGodzilla_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = g_get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			g_build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		g_build_history_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		g_delete_node_at_index(&(info->history), 0);
	g_renumber_history(info);
	return (info->histcount);
}

/**
 * g_build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int g_build_history_list(infoGodzilla_t *info, char *buf, int linecount)
{
	listGodzilla_t *node = NULL;

	if (info->history)
		node = info->history;
	g_add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * g_renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int g_renumber_history(infoGodzilla_t *info)
{
	listGodzilla_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->number = i++;
		node = node->next;
	}
	return (info->histcount = i);
}

