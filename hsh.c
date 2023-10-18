#include "shell.h"

/**
 * g_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int g_hsh(infoGodzilla_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		g_clear_info(info);
		if (g_interactive(info))
			g_puts("$ ");
		g_eputchar(BUFFER_FLUSH);
		r = g_get_input(info);
		if (r != -1)
		{
			g_set_info(info, av);
			builtin_ret = g_find_builtin(info);
			if (builtin_ret == -1)
				g_find_cmd(info);
		}
		else if (g_interactive(info))
			g_putchar('\n');
		g_free_info(info, 0);
	}
	g_write_history(info);
	g_free_info(info, 1);
	if (!g_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->error_num == -1)
			exit(info->status);
		exit(info->error_num);
	}
	return (builtin_ret);
}

/**
 * g_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int g_find_builtin(infoGodzilla_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", g_exit},
		{"env", g_env},
		{"help", g_help},
		{"history", g_history},
		{"setenv", g_mysetenv},
		{"unsetenv", g_myunsetenv},
		{"cd", g_cd},
		{"alias", g_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (g_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * g_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void g_find_cmd(infoGodzilla_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!g_is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, g_getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		g_fork_cmd(info);
	}
	else
	{
		if ((g_interactive(info) || g_getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && g_is_cmd(info, info->argv[0]))
			g_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_g_error(info, "not found\n");
		}
	}
}

/**
 * g_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void g_fork_cmd(infoGodzilla_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, g_get_environ(info)) == -1)
		{
			g_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_g_error(info, "Permission denied\n");
		}
	}
}

