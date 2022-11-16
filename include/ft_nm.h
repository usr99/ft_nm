/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:39:12 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/15 22:19:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _NM_H_
# define _NM_H_

typedef enum e_ft_nm_error
{
	FILE_MAP_FAIL,
	BAD_FILE_FORMAT,
	NO_SYMBOLS,
	SUCCESS
} t_ft_nm_error;

typedef struct 
{
	Elf64_Off offset;
	Elf64_Half entry_size;
	Elf64_Half entry_count;
} t_sections_info;

#endif
