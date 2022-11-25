/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:30:19 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/24 08:07:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
** N for .debug_str
** find the right settings to determine n/N
*/
char detect_symbol_type(t_symbols* sym, t_sections* sections)
{
	if (sym->binding == STB_WEAK)
		return sym->addr ? 'W' : 'w';
	
	char type;
    if (sym->shndx == SHN_ABS)
        type = 'A';
    else if (sym->shndx == SHN_COMMON)
        return ('C');
	else
	{
		t_shdr* shdr = sections->headers + sym->shndx;
		if (shdr->type == SHT_NULL || shdr->flags & SHF_ALLOC)
		{
			if (shdr->flags & SHF_EXECINSTR)
				type = 'T';
			else if (shdr->flags & SHF_WRITE)
			{
				if (shdr->type == SHT_NOBITS)
					type = 'B';
				else if (
					shdr->type == SHT_PROGBITS ||
					shdr->type == SHT_DYNAMIC ||
					shdr->type == SHT_INIT_ARRAY ||
					shdr->type == SHT_FINI_ARRAY
				)	type = 'D';
				else
					return '?';
			}
			else if (!sym->addr)
				type = 'U';
			else
				type = 'R';
		}
		else
			return (shdr->flags & SHF_STRINGS) ? 'n' : 'N';
	}
	return sym->binding == STB_GLOBAL ? type : type + 32;
}
