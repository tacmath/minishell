#ifndef MINISHELL_h
#define MINISHELL_h

#include <unistd.h>
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

struct s_shell
{
	char *home;
	char **shell_env;
};

typedef struct s_shell t_shell;

#endif
