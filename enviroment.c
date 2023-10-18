#include "shell.h"

/**
 * g_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int g_env(infoGodzilla_t *info)
{
	g_print_list_str(info->env);
	return (0);
}

/**
 * g_getenv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *g_getenv(infoGodzilla_t *info, const char *name)
{
	listGodzilla_t *node = info->env;
	char *p;

	while (node)
	{
		p = g_starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * g_mysetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int g_mysetenv(infoGodzilla_t *info)
{
	if (info->argc != 3)
	{
		g_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (g_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * g_myunsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int g_myunsetenv(infoGodzilla_t *info)
{
	int i;

	if (info->argc == 1)
	{
		g_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		g_unsetenv(info, info->argv[i]);

	return (0);
}

/**
 * g_populate_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int g_populate_env_list(infoGodzilla_t *info)
{
	listGodzilla_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		g_add_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}

