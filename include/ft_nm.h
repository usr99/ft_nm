/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:39:12 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/19 07:40:52 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _NM_H_
# define _NM_H_

# include <stdbool.h>
# include <stdlib.h>
# include <elf.h>

typedef enum
{
	FILE_MAP_FAIL,
	BAD_FILE_FORMAT,
	NO_SYMBOLS,
	OOM,
	OUT_OF_BOUNDS,
	SUCCESS
}	t_ft_nm_error;

typedef struct
{
	void*		start;
	Elf64_Half	entry_size;
	Elf64_Half	entry_count;
}				t_shdr_table;

typedef struct
{
	t_shdr_table	shdrtab;
	void*			start;
	size_t 			size;
	bool			x64;
}					t_elf_file;

typedef struct
{
	void*		symbols;
	void*		names;
	Elf64_Xword	symcount;
	Elf64_Xword symsize;
}				t_symbol_table;

#endif
