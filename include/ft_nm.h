/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:39:12 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/19 20:30:30 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _NM_H_
# define _NM_H_

# include <stddef.h>
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
	BAD_OPTION,
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
	Elf64_Section	last_symtab_ndx;
}					t_elf_file;

typedef struct
{
	void*		symbols;
	void*		names;
	Elf64_Xword	symcount;
	Elf64_Xword symsize;
	Elf64_Word	strtab_end;
}				t_symbol_table;

typedef struct s_symbols
{
	Elf64_Addr			addr;
	Elf64_Word			name;
	unsigned char		type;
	unsigned char		binding;
	Elf64_Section		shndx;
	struct s_symbols*	next;
}						t_symbols;

typedef enum
{
	SYMSORT_DEFAULT,
	SYMSORT_REVERSE,
	SYMSORT_DISABLED
}	t_symsort;

typedef struct
{
	const char**	filenames;
	int				fcount;

	t_symsort		sort;
	bool			show_debug_syms;
	bool			extern_only;
	bool			undefined_only;
}					t_options;

/* sections management */
bool			load_section_headers(t_elf_file* binary);
Elf64_Shdr*		load_section_by_index(const t_elf_file* binary, Elf64_Section idx);
t_symbol_table*	load_next_symtab(t_elf_file* binary, t_symbol_table* symtab, bool* error, Elf64_Word tab);

/* List helpers */
t_symbols*	create_list(int symbols_count);
int			list_size(t_symbols* symbols);
void		print_list(t_symbols* symbols, t_symbol_table* symtab, t_elf_file *bin, t_options* params);
void		load_list_64bits(t_symbol_table* symtab, t_symbols* symbols);
void		load_list_32bits(t_symbol_table* symtab, t_symbols* symbols);
void		sort_list(t_symbols* symbols, t_symbol_table* symtab, bool reverse);
void		ft_putnbr_hex(size_t nbr);
char		detect_symbol_type(t_symbols* sym, const t_elf_file* binary);

/* Program options parsing */
t_ft_nm_error parse_options(int argc, char** argv, t_options* opt);

#endif
