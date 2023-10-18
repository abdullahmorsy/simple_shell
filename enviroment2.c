#include "shell.h"

/**
 * g_get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **g_get_environ(infoGodzilla_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = g_list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * g_unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int g_unsetenv(infoGodzilla_t *info, char *var)
{
	listGodzilla_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = g_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = g_delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * g_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int g_setenv(infoGodzilla_t *info, char *var, char *value)
{
	char *buf = NULL;
	listGodzilla_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(g_strlen(var) + g_strlen(value) + 2);
	if (!buf)
		return (1);
	g_strcpy(buf, var);
	g_strcat(buf, "=");
	g_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = g_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	g_add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

