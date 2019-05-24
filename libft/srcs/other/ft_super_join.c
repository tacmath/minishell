/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_super_join.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/20 14:06:10 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/20 14:17:56 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include "libft.h"

static void	join_all_arg(int nb_arg, char *tmp, char **arg)
{
	int m;
	int n;
	int i;

	n = 0;
	i = -1;
	while (++i < nb_arg)
	{
		m = -1;
		while (arg[i][++m])
			tmp[n++] = arg[i][m];
	}
	tmp[n] = 0;
}

char		*ft_super_join(int nb_arg, ...)
{
	va_list	ap;
	char	*tmp;
	char	**arg;
	int		n;
	int		m;

	va_start(ap, nb_arg);
	if (!(arg = malloc(sizeof(char*) * nb_arg)))
		return (0);
	n = -1;
	while (++n < nb_arg)
		arg[n] = va_arg(ap, char*);
	va_end(ap);
	m = 0;
	n = -1;
	while (++n < nb_arg)
		m += ft_strlen(arg[n]);
	if (!(tmp = malloc(sizeof(char) * (m + 1))))
		return (0);
	join_all_arg(nb_arg, tmp, arg);
	free(arg);
	return (tmp);
}
