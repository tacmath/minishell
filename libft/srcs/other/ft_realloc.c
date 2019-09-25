/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_realloc.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/20 14:07:14 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/20 14:18:12 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_realloc(void *mem, int old_len, int new_len)
{
	char	*tmp;
	char	*str;
	int		n;

	if (!(tmp = malloc(sizeof(char) * new_len)))
		return (0);
	str = (char*)mem;
	n = -1;
	while (++n < old_len && n < new_len)
		tmp[n] = str[n];
	return (tmp);
}
