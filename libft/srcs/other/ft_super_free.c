/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_super_free.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/20 14:05:40 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/20 14:05:53 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>

int	ft_super_free(int nb_arg, ...)
{
	va_list	ap;

	va_start(ap, nb_arg);
	while (nb_arg--)
		free(va_arg(ap, void*));
	va_end(ap);
	return (0);
}
