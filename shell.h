#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for g_convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @number: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int number;
	char *str;
	struct liststr *next;
} listGodzilla_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@error_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int error_num;
	int linecount_flag;
	char *fname;
	listGodzilla_t *env;
	listGodzilla_t *history;
	listGodzilla_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} infoGodzilla_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(infoGodzilla_t *);
} builtin_table;


/* hsh.c */
int g_hsh(infoGodzilla_t *, char **);
int g_find_builtin(infoGodzilla_t *);
void g_find_cmd(infoGodzilla_t *);
void g_fork_cmd(infoGodzilla_t *);

/* path.c */
int g_is_cmd(infoGodzilla_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(infoGodzilla_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* error_str_funcs.c */
void g_eputs(char *);
int g_eputchar(char);
int g_putfd(char c, int fd);
int g_putsfd(char *str, int fd);

/* str_funcs.c */
int g_strlen(char *);
int g_strcmp(char *, char *);
char *g_starts_with(const char *, const char *);
char *g_strcat(char *, char *);

/* str_funcs2.c */
char *g_strcpy(char *, char *);
char *_strdup(const char *);
void g_puts(char *);
int g_putchar(char);

/* str_funcs3.c */
char *g_strncpy(char *, char *, int);
char *g_strncat(char *, char *, int);
char *g_strchr(char *, char);

/* str_funcs4.c */
char **g_strtow(char *, char *);
char **g_strtow2(char *, char);

/* memory_funcs */
char *g_memset(char *, char, unsigned int);
void g_ffree(char **);
void *g_realloc(void *, unsigned int, unsigned int);

/* memory_funcs2.c */
int g_bfree(void **);

/* more_funcs.c */
int g_interactive(infoGodzilla_t *);
int g_is_delim(char, char *);
int g_isalpha(int);
int g_atoi(char *);

/* more_funcs2.c */
int g_erratoi(char *);
void print_g_error(infoGodzilla_t *, char *);
int g_print_d(int, int);
char *g_convert_number(long int, int, int);
void remove_g_comments(char *);

/* builtin_emuls.c */
int g_exit(infoGodzilla_t *);
int g_cd(infoGodzilla_t *);
int g_help(infoGodzilla_t *);

/* builtin_emuls2.c */
int g_history(infoGodzilla_t *);
int g_alias(infoGodzilla_t *);

/* getline.c module */
ssize_t g_get_input(infoGodzilla_t *);
int g_getline(infoGodzilla_t *, char **, size_t *);
void g_sigintHandler(int);

/* info.c module */
void g_clear_info(infoGodzilla_t *);
void g_set_info(infoGodzilla_t *, char **);
void g_free_info(infoGodzilla_t *, int);

/* enviroment.c module */
char *g_getenv(infoGodzilla_t *, const char *);
int g_env(infoGodzilla_t *);
int g_mysetenv(infoGodzilla_t *);
int g_myunsetenv(infoGodzilla_t *);
int g_populate_env_list(infoGodzilla_t *);

/* enviroment2.c module */
char **g_get_environ(infoGodzilla_t *);
int g_unsetenv(infoGodzilla_t *, char *);
int g_setenv(infoGodzilla_t *, char *, char *);

/* file_io_funcs.c */
char *g_get_history_file(infoGodzilla_t *info);
int g_write_history(infoGodzilla_t *info);
int g_read_history(infoGodzilla_t *info);
int g_build_history_list(infoGodzilla_t *info, char *buf, int linecount);
int g_renumber_history(infoGodzilla_t *info);

/* liststring.c module */
listGodzilla_t *g_add_node(listGodzilla_t **, const char *, int);
listGodzilla_t *g_add_node_end(listGodzilla_t **, const char *, int);
size_t g_print_list_str(const listGodzilla_t *);
int g_delete_node_at_index(listGodzilla_t **, unsigned int);
void g_free_list(listGodzilla_t **);

/* liststring2.c module */
size_t g_list_len(const listGodzilla_t *);
char **g_list_to_strings(listGodzilla_t *);
size_t g_print_list(const listGodzilla_t *);
listGodzilla_t *g_node_starts_with(listGodzilla_t *, char *, char);
ssize_t g_get_node_index(listGodzilla_t *, listGodzilla_t *);

/* chain.c */
int g_is_chain(infoGodzilla_t *, char *, size_t *);
void g_check_chain(infoGodzilla_t *, char *, size_t *, size_t, size_t);
int g_replace_alias(infoGodzilla_t *);
int g_replace_vars(infoGodzilla_t *);
int g_replace_string(char **, char *);

#endif

