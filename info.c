#include "shell.h"

/**
 * g_clear_info - initializes infoGodzilla_t struct
 * @info: struct address
 */
void g_clear_info(infoGodzilla_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * g_set_info - initializes infoGodzilla_t struct
 * @info: struct address
 * @av: argument vector
 */
void g_set_info(infoGodzilla_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = g_strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		g_replace_alias(info);
		g_replace_vars(info);
	}
}

/**
 * g_free_info - frees infoGodzilla_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void g_free_info(infoGodzilla_t *info, int all)
{
	g_ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			g_free_list(&(info->env));
		if (info->history)
			g_free_list(&(info->history));
		if (info->alias)
			g_free_list(&(info->alias));
		g_ffree(info->environ);
			info->environ = NULL;
		g_bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		g_putchar(BUFFER_FLUSH);
	}
}

