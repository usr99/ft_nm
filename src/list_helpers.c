/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:04:03 by timlecou          #+#    #+#             */
/*   Updated: 2022/11/19 18:33:33 by kali             ###   ########.fr       */
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
        tmp = tmp->next;
    }
}