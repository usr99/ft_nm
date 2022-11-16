/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:38:42 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/16 15:48:46 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "ft_nm.h"
#include "libft.h"

static const char* map_file_content(const char* filename, off_t* size)
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
	
	const char* mapped = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	
	close(fd);
	*size = info.st_size;
	return mapped;
}

static int fatal(t_ft_nm_error code, const char* prefix)
{
	static const char* errmsgs[] = {
		"failed to map file in memory",
		"file format not recognized",
		"no symbols"
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

static t_sections_info find_sections_table(Elf64_Ehdr* elfhdr)
{
	t_sections_info info;

	if (elfhdr->e_ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr* elfhdr32 = (Elf32_Ehdr*)elfhdr;
		info.offset = elfhdr32->e_shoff;
		info.entry_size = elfhdr32->e_shentsize;
		info.entry_count = elfhdr32->e_shnum;
	}
	else
	{
		info.offset = elfhdr->e_shoff;
		info.entry_size = elfhdr->e_shentsize;
		info.entry_count = elfhdr->e_shnum;
	}
	return info;
}

int main(int argc, char** argv)
{
	const char* fname = "a.out";
	if (argc > 1)
		fname = argv[1];

	off_t size;
	const char* fcontent = map_file_content(fname, &size);
	if (fcontent == MAP_FAILED)
		return fatal(FILE_MAP_FAIL, fname);

	if (!check_binary_format((Elf64_Ehdr*)fcontent))
	{
		munmap((void*)fcontent, size);
		return fatal(BAD_FILE_FORMAT, fname);
	}

	t_sections_info sections = find_sections_table((Elf64_Ehdr*)fcontent);
	Elf64_Shdr* sec = (void*)fcontent + sections.offset; // should check offset to avoid segfault
	
	int i;
	for (i = 0; i < sections.entry_count && sec->sh_type != SHT_SYMTAB; i++)
		sec = (void*)sec + sections.entry_size;

	if (i != sections.entry_count)
	{
	}
	else
		fatal(NO_SYMBOLS, fname);

	munmap((void*)fcontent, size);
	return EXIT_SUCCESS;
}
