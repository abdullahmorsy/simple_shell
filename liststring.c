#include "shell.h"

/**
 * g_add_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @number: node index used by history
 *
 * Return: size of list
 */
listGodzilla_t *g_add_node(listGodzilla_t **head, const char *str, int number)
{
	listGodzilla_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(listGodzilla_t));
	if (!new_head)
		return (NULL);
	g_memset((void *)new_head, 0, sizeof(listGodzilla_t));
	new_head->number = number;
	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * g_add_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @number: node index used by history
 *
 * Return: size of list
 */
listGodzilla_t *g_add_node_end(listGodzilla_t **head,
		const char *str, int number)
{
	listGodzilla_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(listGodzilla_t));
	if (!new_node)
		return (NULL);
	g_memset((void *)new_node, 0, sizeof(listGodzilla_t));
	new_node->number = number;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * g_print_list_str - prints only the str elem of a listGodzilla_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t g_print_list_str(const listGodzilla_t *h)
{
	size_t i = 0;

	while (h)
	{
		g_puts(h->str ? h->str : "(nil)");
		g_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * g_delete_node_at_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int g_delete_node_at_index(listGodzilla_t **head, unsigned int index)
{
	listGodzilla_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * g_free_list - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void g_free_list(listGodzilla_t **head_ptr)
{
	listGodzilla_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}

