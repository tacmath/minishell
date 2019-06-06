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
	char *last_dir;
	char **shell_env;
};

typedef struct s_shell t_shell;
int run_non_builtin(char **av, t_shell *shell);
int change_env(char **env, char *var, char *new);
int ft_cd(char **av, t_shell *shell);
int ft_setenv(char **av, t_shell *shell);
int ft_unsetenv(char **av, t_shell *shell);
int run_builtins(char **av, t_shell *shell);
int shell_init(t_shell *shell);
char **get_av(char *line);
int treat_av(char **av, t_shell *shell);

#endif
