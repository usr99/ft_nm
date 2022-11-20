/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:04:03 by timlecou          #+#    #+#             */
/*   Updated: 2022/11/19 19:53:48 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "ft_nm.h"
#include "libft.h"

t_symbols*  create_list(int symbols_count)
{
    t_symbols*  sym = mmap(NULL, symbols_count * sizeof(sym), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    int i = 0;
    t_symbols*  tmp = sym;
    while (i < symbols_count)
    {
        tmp->next = tmp + 1;
        i++;
        tmp = tmp->next;
    }
    tmp->next = NULL;
    return (sym);
}

int     list_size(t_symbols* symbols)
{
    int i = 0;
    t_symbols*  tmp = symbols;

    while (tmp->next)
    {
        tmp = tmp->next;
        i++;
    }
    return (i);
}

void	load_list_32bits(t_symbol_table* symtab, t_symbols* symbols)
{
	t_symbols*  tmp = symbols;
    Elf32_Sym*  syms = symtab->symbols;
    Elf32_Xword i = 0;

    while (i < symtab->symcount)
    {
        if (syms->st_name != 0 && ELF32_ST_TYPE(syms->st_info) != STT_FILE)
        {
            tmp->name = syms->st_name;
            tmp->addr = syms->st_value;
            tmp->type = ELF32_ST_TYPE(syms->st_info);
            tmp->binding = ELF32_ST_BIND(syms->st_info);
            tmp->shndx = syms->st_shndx;
            tmp = tmp->next;
        }
        syms = syms + 1;
        i++;
    }
    tmp->next = NULL;
}

void	load_list_64bits(t_symbol_table* symtab, t_symbols* symbols)
{
	t_symbols*  tmp = symbols;
    Elf64_Sym*  syms = symtab->symbols;
    Elf64_Xword i = 0;

    while (i < symtab->symcount)
    {
        if (syms->st_name != 0 && ELF64_ST_TYPE(syms->st_info) != STT_FILE)
        {
            tmp->name = syms->st_name;
            tmp->addr = syms->st_value;
            tmp->type = ELF64_ST_TYPE(syms->st_info);
            tmp->binding = ELF64_ST_BIND(syms->st_info);
            tmp->shndx = syms->st_shndx;
            tmp = tmp->next;
        }
        syms = syms + 1;
        i++;
    }
    tmp->next = NULL;
}

void ft_putnbr_hex(size_t nbr)
{
    const char* digits = "0123456789abcdef";

    if (nbr >= 16)
    {
        ft_putnbr_hex(nbr / 16);
        ft_putchar_fd(digits[nbr % 16], 1);
    }
    else
        ft_putchar_fd(digits[nbr % 16], 1);
}

void    print_zeros(size_t nbr, short arch)
{
    int     i = 1;
    while (nbr >= 16)
    {
        nbr /= 16;
        i++;
    }
    write(1, "0000000000000000", arch - i);
}

void    print_symbols(char* names, t_symbol_table* symtab)
{
    unsigned int    i = 0;

    while (names[i] != '\0' && i < symtab->strtab_end)
        i++;
    write(1, names, i);
    write(1, "\n", 1);
}

void    print_list(t_symbols* symbols, t_symbol_table* symtab, t_elf_file* bin)
{
    t_symbols*  tmp = symbols;
    char        type = 0;

    while (tmp->next)
    {
        if (tmp->addr != 0)
        {
            print_zeros(tmp->addr, (bin->x64 ? 16 : 8));
            ft_putnbr_hex(tmp->addr);
        }
        else
        {
            if (bin->x64)
                write(1, "                ", 16);
            else
                write(1, "        ", 8);
        }
        write(1, " ", 1);
        type = detect_symbol_type(tmp, bin);
        write(1, &type, 1);
        write(1, " ", 1);
        print_symbols(symtab->names + tmp->name, symtab);
        // printf("%d\n", tmp->shndx);
        tmp = tmp->next;
    }
}

void	swap_names(t_symbols* node_1, t_symbols* node_2)
{
    Elf64_Word	tmp1 = node_1->name;
    node_1->name = node_2->name;
    node_2->name = tmp1;
}

void	swap_addresses(t_symbols* node_1, t_symbols* node_2)
{
    Elf64_Addr	tmp1 = node_1->addr;
    node_1->addr = node_2->addr;
    node_2->addr = tmp1;
}

void	swap_types(t_symbols* node_1, t_symbols* node_2)
{
    unsigned char	tmp1 = node_1->type;
    node_1->type = node_2->type;
    node_2->type = tmp1;
}

void	swap_bindings(t_symbols* node_1, t_symbols* node_2)
{
    unsigned char	tmp1 = node_1->binding;
    node_1->binding = node_2->binding;
    node_2->binding = tmp1;
}

void	swap_shndxs(t_symbols* node_1, t_symbols* node_2)
{
    Elf64_Section	tmp1 = node_1->shndx;
    node_1->shndx = node_2->shndx;
    node_2->shndx = tmp1;
}

int	ft_strncmp_no_case(const char *s1, const char *s2, size_t n)
{
	size_t	i;
    char    c1;
    char    c2;

	i = 0;
	if (n == 0)
		return (0);
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


void	sort_list(t_symbols* symbols, t_symbol_table* symtab)
{
	int	curr_i;
	int	next_i;
	t_symbols*	tmp = symbols;
	t_symbols*	tmp_2 = symbols->next;
    char*       current_name = NULL;
    char*       next_name = NULL;

    while (tmp->next)
    {
        tmp_2 = tmp->next;
        while (tmp_2->next)
        {
            curr_i = 0;
            next_i = 0;
            current_name = symtab->names + tmp->name;
            next_name = symtab->names + tmp_2->name;
            while (current_name[curr_i] != '\0' && current_name[curr_i] == '_')
                curr_i++;
            while (next_name[next_i] != '\0' && next_name[next_i] == '_')
                next_i++;
            if (ft_strncmp_no_case(&current_name[curr_i], &next_name[next_i], ft_strlen(&next_name[next_i])) >= 0)
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