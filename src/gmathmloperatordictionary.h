/* gmathmloperatordictionary.h
 *
 * Copyright (C) 2007  Emmanuel Pacaud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef GMATHML_OPERATOR_DICTIONARY_H
#define GMATHML_OPERATOR_DICTIONARY_H

#include <gmathml.h>
#include <gmathmlenums.h>

G_BEGIN_DECLS

typedef struct {
	const char 		*name;
	GMathmlForm		 form;
	GMathmlNamedSpace	 l_space;
	GMathmlNamedSpace	 r_space;
	gboolean		 stretchy;
	gboolean		 fence;
	gboolean		 accent;
	gboolean		 large_op;
	gboolean		 movable_limits;
	gboolean		 separator;
	int			 min_size;
	int			 max_size;
	gboolean		 symmetric;
} GMathmlOperator;

const GMathmlOperator *		gmathml_operator_get_attributes (const char *utf8, GMathmlForm form);

G_END_DECLS

#endif
