#include "shell.h"

/**
 * g_is_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int g_is_chain(infoGodzilla_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * g_check_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void g_check_chain(infoGodzilla_t *info,
		char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * g_replace_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int g_replace_alias(infoGodzilla_t *info)
{
	int i;
	listGodzilla_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = g_node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = g_strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * g_replace_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int g_replace_vars(infoGodzilla_t *info)
{
	int i = 0;
	listGodzilla_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!g_strcmp(info->argv[i], "$?"))
		{
			g_replace_string(&(info->argv[i]),
				_strdup(g_convert_number(info->status, 10, 0)));
			continue;
		}
		if (!g_strcmp(info->argv[i], "$$"))
		{
			g_replace_string(&(info->argv[i]),
				_strdup(g_convert_number(getpid(), 10, 0)));
			continue;
		}
		node = g_node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			g_replace_string(&(info->argv[i]),
				_strdup(g_strchr(node->str, '=') + 1));
			continue;
		}
		g_replace_string(&info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * g_replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int g_replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

