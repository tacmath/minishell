
#include "minishell.h"

void    remove_one_char(char *line1, char *line2)
{
    int len;
    
    len = ft_strlen(line1);
    if (len)
    {
        line1[len - 1] = 0;
        tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)), 1, oputchar);
        tputs(tgetstr("ce", 0), 1, oputchar);
        write(1, line2, ft_strlen(line2));
        tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)), 1, oputchar);
    }
}

int go_to_right(char **line1, char **line2)
{
    int len;
    char *tmp;
    
    len = ft_strlen(*line2);
    if (len)
    {
        len = ft_strlen(*line1);
        if (!(tmp = ft_memalloc(len + 2)))
        {
            ft_super_free(2, *line1, *line2);
            return (0);
        }
        ft_strcpy(tmp, *line1);
        tmp[len] = (*line2)[0];
        free(*line1);
        *line1 = tmp;
        ft_strcpy(*line2, &(*line2)[1]);
        tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
    }
    return (1);
}

int go_to_left(char **line1, char **line2)
{
    int len1;
    int len2;
    char *tmp;
    
    len1 = ft_strlen(*line1);
    if (len1)
    {
        len2 = ft_strlen(*line2);
        if (!(tmp = ft_memalloc(len2 + 2)))
        {
            ft_super_free(2, *line1, *line2);
            return (0);
        }
        ft_strcpy(&tmp[1], *line2);
        tmp[0] = (*line1)[len1 - 1];
        (*line1)[len1 - 1] = 0;
        free(*line2);
        *line2 = tmp;
        tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
    }
    return (1);
}
