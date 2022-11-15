/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:38:42 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/15 12:56:01 by mamartin         ###   ########.fr       */
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
		"not an elf file",
		"invalid class. Only 32 and 64-bits are accepted",
		"invalid endian parameter",
		"invalid elf version",
		"invalid object type. Only relocatables, executables and shared libraries are accepted",
		"invalid architecture. Only x86_32 and x64 are accepted"
	};

	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsgs[code], STDERR_FILENO);
	return EXIT_FAILURE;
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
	munmap((void*)fcontent, size);
	return EXIT_SUCCESS;
}
