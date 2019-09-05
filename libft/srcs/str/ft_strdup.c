/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strdup.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/08/08 10:43:04 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 16:58:11 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *src)
{
	int		taille;
	char	*str;

	taille = -1;
	while (src[++taille])
		;
	if (!(str = malloc(sizeof(char) * (taille + 1))))
		return (0);
	taille = 0;
	while (src[taille])
	{
		str[taille] = src[taille];
		taille++;
	}
	str[taille] = '\0';
	return (str);
}
