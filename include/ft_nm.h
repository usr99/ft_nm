/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:39:12 by mamartin          #+#    #+#             */
/*   Updated: 2022/11/15 12:48:38 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _NM_H_
# define _NM_H_

typedef enum e_ft_nm_error
{
	FILE_MAP_FAIL,
	BAD_MAGIC_NUMBER,
	INVALID_CLASS,
	INVALID_ENDIAN,
	INVALID_VERSION,
	INVALID_OBJ_TYPE,
	INVALID_ARCHITECTURE,
	SUCCESS
} t_ft_nm_error;

#endif
