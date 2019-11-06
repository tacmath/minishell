/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_files.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:01:11 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 16:28:45 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		get_all_command_from_path(t_file **list, char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;
	int				path_len;
	t_file			tmp;

	command_len = ft_strlen(command);
	if (!(dir = opendir(path)))
		return (0);
	path_len = ft_strlen(path);
	if (!(path = ft_realloc(path, path_len, path_len + sizeof(info->d_name))))
		return (0);
	while ((info = readdir(dir)))
		if (info->d_type == DT_REG
			&& !ft_strncmp(command, info->d_name, command_len)
			&& ft_strcpy(&path[path_len], info->d_name)
			&& !access(path, X_OK))
		{
			tmp.name = ft_strdup(info->d_name);
			tmp.type = -2;
			if (!add_to_list(list, tmp))
				return (0);
		}
	closedir(dir);
	return (!ft_super_free(1, path));
}

int		check_command(char *command, struct dirent *info, int command_len)
{
	if ((command[0] == '.' || info->d_name[0] != '.')
			&& !ft_strncmp(command, info->d_name, command_len))
		return (1);
	return (0);
}

int		get_all_command_and_dir_from_path(t_file **list,
		char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;
	int				path_len;
	t_file			tmp;

	command_len = ft_strlen(command);
	path_len = ft_strlen(path);
	if (!(dir = opendir(path)) ||
		!(path = ft_realloc(path, path_len, path_len + sizeof(info->d_name))))
		return (0);
	while ((info = readdir(dir)))
		if (check_command(command, info, command_len)
			&& (info->d_type == DT_DIR || (info->d_type == DT_REG &&
			ft_strcpy(&path[path_len], info->d_name) && !access(path, X_OK))))
		{
			tmp.name = ft_strdup(info->d_name);
			if (info->d_type == DT_DIR)
				tmp.type = info->d_type;
			else
				tmp.type = -1;
			if (!add_to_list(list, tmp))
				return (0);
		}
	return (!ft_super_free(1, path) && !closedir(dir));
}

int		get_all_command(t_shell *shell, char *command)
{
	static char	builtin[7][9] = {"cd", "env",
		"echo", "setenv", "unsetenv", "exit"};
	char		**path;
	int			n;
	int			command_len;
	t_file		tmp;

	tmp.type = -2;
	command_len = ft_strlen(command);
	n = -1;
	while (++n < 6)
		if (!ft_strncmp(command, builtin[n], command_len))
		{
			tmp.name = ft_strdup(builtin[n]);
			if (!add_to_list(&shell->comp->list, tmp))
				return (0);
		}
	if (!(path = get_all_path(shell->shell_env)))
		return (0);
	n = -1;
	while (path[++n])
		if (!get_all_command_from_path(&shell->comp->list, path[n], command))
			return (0);
	free_av(path);
	return (1);
}

int		get_all_from_path(t_file **list, char *path, char *start)
{
	DIR				*dir;
	struct dirent	*info;
	int				start_len;
	int				path_len;
	t_file			tmp;

	start_len = ft_strlen(start);
	path_len = ft_strlen(path);
	if (!(dir = opendir(path)) ||
		!(path = ft_realloc(path, path_len, path_len + sizeof(info->d_name))))
		return (0);
	while ((info = readdir(dir)))
		if ((start[0] == '.' || info->d_name[0] != '.')
				&& !ft_strncmp(start, info->d_name, start_len))
		{
			ft_strcpy(&path[path_len], info->d_name);
			tmp.name = ft_strdup(info->d_name);
			if (access(path, X_OK) || info->d_type == DT_DIR)
				tmp.type = info->d_type;
			if (!add_to_list(list, tmp))
				return (0);
		}
	return (!ft_super_free(1, path) && !closedir(dir));
}
