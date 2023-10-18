#include "shell.h"

/**
 * g_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int g_history(infoGodzilla_t *info)
{
	g_print_list(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(infoGodzilla_t *info, char *str)
{
	char *p, c;
	int ret;

	p = g_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = g_delete_node_at_index(&(info->alias),
		g_get_node_index(info->alias, g_node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(infoGodzilla_t *info, char *str)
{
	char *p;

	p = g_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (g_add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(listGodzilla_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = g_strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			g_putchar(*a);
		g_putchar('\'');
		g_puts(p + 1);
		g_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * g_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int g_alias(infoGodzilla_t *info)
{
	int i = 0;
	char *p = NULL;
	listGodzilla_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = g_strchr(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(g_node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}

