/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:39 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 12:54:18 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int	change_env(char **env, char *var, char *new)
{
	int n;

	n = -1;
	while (env[++n])
		if (!ft_strncmp(env[n], var, ft_strlen(var))
			&& env[n][ft_strlen(var)] == '=')
		{
			free(env[n]);
			env[n] = ft_super_join(3, var, "=", new);
			break ;
		}
	return (1);
}

int	env_init(t_shell *shell, char **env)
{
	int n;

	n = -1;
	while (env[++n])
		;
	if (!(shell->shell_env = ft_memalloc(sizeof(char*) * ++n)))
		return (0);
	n = -1;
	while (env[++n])
		shell->shell_env[n] = ft_strdup(env[n]);
	return (1);
}

int	shell_init(t_shell *shell, char **env, char *name)
{
	int		n;
	char	*tmp;
	char	*path_tmp;

	shell->home = 0;
	n = -1;
	while (env[++n])
		if (!ft_strncmp(env[n], "HOME=", 5))
			if (!(shell->home = ft_strdup(&env[n][5])))
				return (0);
	if (!shell->home && (shell->home = getcwd(0, 0)))
		return (0);
	shell->last_dir = 0;
	env_init(shell, env);
	if (name[0] == '.')
	{
		path_tmp = getcwd(0, 0);
		tmp = ft_super_join(3, path_tmp, "/", name);
		free(path_tmp);
	}
	else if (!(tmp = ft_strdup(name)))
		return (0);
	change_env(shell->shell_env, "SHELL", tmp);
	free(tmp);
	return (1);
}
