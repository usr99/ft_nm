/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 05:14:03 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/25 05:18:25 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include "libft.h"

static int	ft_strncmp_no_case(const char *s1, const char *s2, size_t n)
{
	size_t	i;
    char    c1;
    char    c2;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
        c1 = s1[i];
        c2 = s2[i];
        if (c1 >= 65 && c1 <= 90)
            c1 += 32;
        if (c2 >= 65 && c2 <= 90)
            c2 += 32;
		if (c1 != c2)
			return ((unsigned char)c1 - (unsigned char)c2);
		i++;
	}
	return (0);
}

static void	swap_names(t_symbols* node_1, t_symbols* node_2)
{
    char*	tmp1 = node_1->name;
    node_1->name = node_2->name;
    node_2->name = tmp1;
}

static void	swap_addresses(t_symbols* node_1, t_symbols* node_2)
{
    Elf64_Addr	tmp1 = node_1->addr;
    node_1->addr = node_2->addr;
    node_2->addr = tmp1;
}

static void	swap_types(t_symbols* node_1, t_symbols* node_2)
{
    unsigned char	tmp1 = node_1->type;
    node_1->type = node_2->type;
    node_2->type = tmp1;
}

static void	swap_bindings(t_symbols* node_1, t_symbols* node_2)
{
    unsigned char	tmp1 = node_1->binding;
    node_1->binding = node_2->binding;
    node_2->binding = tmp1;
}

static void	swap_shndxs(t_symbols* node_1, t_symbols* node_2)
{
    Elf64_Section	tmp1 = node_1->shndx;
    node_1->shndx = node_2->shndx;
    node_2->shndx = tmp1;
}

/*
** Split on '.'/'_'
** Sort by "most separators"
** case insensitive but UPPERCASE < lowercase
*/
void	sort_list(t_symbols* symbols, bool reverse)
{
	int	curr_i;
	int	next_i;
	t_symbols*	tmp = symbols;
	t_symbols*	tmp_2 = symbols->next;
    char*       current_name = NULL;
    char*       next_name = NULL;
    bool        ret = false;

    while (tmp->next)
    {
        tmp_2 = tmp->next;
        while (tmp_2->next)
        {
            curr_i = 0;
            next_i = 0;
			current_name = tmp->name;
			next_name = tmp_2->name;
            while (current_name[curr_i] != '\0' && (current_name[curr_i] == '_' || current_name[curr_i] == '.'))
                curr_i++;
            while (next_name[next_i] != '\0' && (next_name[next_i] == '_' || next_name[next_i] == '.'))
                next_i++;

			size_t max = (ft_strlen(current_name) > ft_strlen(next_name)) ? ft_strlen(current_name) : ft_strlen(next_name);
            if (reverse == false)
			{
				int res = ft_strncmp_no_case(&current_name[curr_i], &next_name[next_i], max);
				if (res == 0 && curr_i != next_i)
					ret = (curr_i < next_i);
				else
					ret = (res >= 0);
			}
			else
			{
				int res = ft_strncmp_no_case(&current_name[curr_i], &next_name[next_i], max);
				if (res == 0 && curr_i != next_i)
					ret = (curr_i > next_i);
				else
					ret = (res < 0);
			}
            if (ret)
            {
                swap_names(tmp, tmp_2);
                swap_addresses(tmp, tmp_2);
                swap_types(tmp, tmp_2);
                swap_bindings(tmp, tmp_2);
                swap_shndxs(tmp, tmp_2);
            }
            tmp_2 = tmp_2->next;
        }
        tmp = tmp->next;
    }
}
