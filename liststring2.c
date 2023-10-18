#include "shell.h"

/**
 * g_list_len - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t g_list_len(const listGodzilla_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * g_list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **g_list_to_strings(listGodzilla_t *head)
{
	listGodzilla_t *node = head;
	size_t i = g_list_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(g_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = g_strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * g_print_list - prints all elements of a listGodzilla_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t g_print_list(const listGodzilla_t *h)
{
	size_t i = 0;

	while (h)
	{
		g_puts(g_convert_number(h->number, 10, 0));
		g_putchar(':');
		g_putchar(' ');
		g_puts(h->str ? h->str : "(nil)");
		g_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * g_node_starts_with - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
listGodzilla_t *g_node_starts_with(listGodzilla_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = g_starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * g_get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t g_get_node_index(listGodzilla_t *head, listGodzilla_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}

