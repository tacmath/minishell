/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_files.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:01:11 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 15:21:39 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int			get_all_command_from_path(char ***list, char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;

	command_len = ft_strlen(command);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if (access(info->d_name, X_OK) &&
			!ft_strncmp(command, info->d_name, command_len))
			if (!add_to_list(list, ft_strdup(info->d_name)))
				return (0);
	}
	closedir(dir);
	return (1);
}

static char	*add_slach_to_name(char *name)
{
	char	*tmp;
	int		len;

	len = ft_strlen(name);
	if (!(tmp = ft_memalloc(sizeof(char) + len + 2)))
		return (0);
	ft_strcpy(tmp, name);
	tmp[len] = '/';
	return (tmp);
}

int			get_all_command_and_dir_from_path(char ***list,
	char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;

	command_len = ft_strlen(command);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if ((command[0] == '.' || info->d_name[0] != '.')
			&& !ft_strncmp(command, info->d_name, command_len))
		{
			if (info->d_type == DT_DIR)
			{
				if (!add_to_list(list, add_slach_to_name(info->d_name)))
					return (0);
			}
			else if (access(info->d_name, X_OK) &&
				!add_to_list(list, ft_strdup(info->d_name)))
				return (0);
		}
	}
	closedir(dir);
	return (1);
}

int			get_all_command(char ***list, char *command)
{
	t_shell		*shell;
	static char	builtin[7][9] = {"cd", "env",
				"echo", "setenv", "unsetenv", "exit"};
	char		**path;
	int			n;
	int			command_len;

	shell = get_shell(0);
	command_len = ft_strlen(command);
	n = -1;
	while (++n < 6)
		if (!ft_strncmp(command, builtin[n], command_len)
			&& !add_to_list(list, ft_strdup(builtin[n])))
			return (0);
	if (!(path = get_all_path(shell->shell_env)))
		return (0);
	n = -1;
	while (path[++n])
		if (!get_all_command_from_path(list, path[n], command))
			return (0);
	free_av(path);
	return (1);
}

int			get_all_from_path(char ***list, char *path, char *start)
{
	DIR				*dir;
	struct dirent	*info;
	int				start_len;

	start_len = ft_strlen(start);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if ((start[0] == '.' || info->d_name[0] != '.')
			&& !ft_strncmp(start, info->d_name, start_len))
		{
			if (info->d_type == DT_DIR)
			{
				if (!add_to_list(list, add_slach_to_name(info->d_name)))
					return (0);
			}
			else if (!add_to_list(list, ft_strdup(info->d_name)))
				return (0);
		}
	}
	closedir(dir);
	return (1);
}
