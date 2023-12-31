#include "shell.h"

/**
 * g_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int g_exit(infoGodzilla_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = g_erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_g_error(info, "Illegal number: ");
			g_eputs(info->argv[1]);
			g_eputchar('\n');
			return (1);
		}
		info->error_num = g_erratoi(info->argv[1]);
		return (-2);
	}
	info->error_num = -1;
	return (-2);
}

/**
 * g_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int g_cd(infoGodzilla_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		g_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = g_getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = g_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (g_strcmp(info->argv[1], "-") == 0)
	{
		if (!g_getenv(info, "OLDPWD="))
		{
			g_puts(s);
			g_putchar('\n');
			return (1);
		}
		g_puts(g_getenv(info, "OLDPWD=")), g_putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = g_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_g_error(info, "can't cd to ");
		g_eputs(info->argv[1]), g_eputchar('\n');
	}
	else
	{
		g_setenv(info, "OLDPWD", g_getenv(info, "PWD="));
		g_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * g_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int g_help(infoGodzilla_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	g_puts("help call works. Function not yet implemented \n");
	if (0)
		g_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}

