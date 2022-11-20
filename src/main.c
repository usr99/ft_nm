/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:38:42 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/19 20:20:30 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "ft_nm.h"
#include "libft.h"

static void* map_file_content(const char* filename, size_t* size)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		return MAP_FAILED;

	struct stat info;
	if (fstat(fd, &info) == -1)
	{
		close(fd);
		return MAP_FAILED;
	}
	
	void* mapped = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	
	close(fd);
	*size = info.st_size;
	return mapped;
}

static int fatal(t_ft_nm_error code, const char* prefix)
{
	static const char* errmsgs[] = {
		"failed to map file in memory",
		"file format not recognized",
		"no symbols",
		"out of memory",
		"pointer outside of mapped content"
	};

	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsgs[code], STDERR_FILENO);
	return EXIT_FAILURE;
}

static bool check_binary_format(Elf64_Ehdr* elfhdr)
{
	return (
		ft_memcmp(ELFMAG, elfhdr->e_ident, 4) == 0 &&
		(elfhdr->e_ident[EI_CLASS] == ELFCLASS32 || elfhdr->e_ident[EI_CLASS] == ELFCLASS64) &&
		(elfhdr->e_ident[EI_DATA] == ELFDATA2LSB || elfhdr->e_ident[EI_DATA] == ELFDATA2MSB) &&
		elfhdr->e_ident[EI_VERSION] == EV_CURRENT &&
		elfhdr->e_type != ET_CORE
	);
}

int main(int argc, char** argv)
{
	t_options params = {0};
	if (argc == 1)
	{
		params.filenames = malloc(sizeof(const char*));
		if (!params.filenames)
			return fatal(OOM, *argv);
	}
	else
	{
		t_ft_nm_error status = parse_options(argc, argv, &params);
		if (status == OOM)
			return fatal(OOM, *argv);
	}

	if (params.fcount == 0)
	{
		*params.filenames = "a.out";
		params.fcount = 1;
	}

	int i = 0;
	while (i < params.fcount)
	{
		const char* fname = params.filenames[i];

		t_elf_file bin = {0};
		bin.start = map_file_content(fname, &bin.size);
		if (bin.start == MAP_FAILED)
			return fatal(FILE_MAP_FAIL, fname);

		if (!check_binary_format(bin.start))
		{
			munmap(bin.start, bin.size);
			return fatal(BAD_FILE_FORMAT, fname);
		}

		if (!load_section_headers(&bin))
		{
			munmap(bin.start, bin.size);
			return fatal(OUT_OF_BOUNDS, fname);
		}

		if (params.fcount != 1)
		{
			ft_putchar('\n');
			ft_putstr(fname);
			ft_putstr(":\n");
		}

		bool err;
		t_symbol_table symtab;
		t_symbols *symbols;
		bool symbols_found = false;
		while (load_next_symtab(&bin, &symtab, &err, SHT_SYMTAB))
		{
			symbols_found = true;
			symbols = create_list(symtab.symcount);
			if (bin.x64)
				load_list_64bits(&symtab, symbols);
			else
				load_list_32bits(&symtab, symbols);
			sort_list(symbols, &symtab, true);
			print_list(symbols, &symtab, &bin);
		}

		munmap(bin.start, bin.size);
		if (err || !symbols_found)
			return fatal(err ? OUT_OF_BOUNDS : NO_SYMBOLS, fname);
		i++;
	}

	free(params.filenames);
	return EXIT_SUCCESS;
}