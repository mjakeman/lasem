/* Lasem
 *
 * Copyright © 2021 Matthew Jakeman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1335, USA.
 *
 * Author:
 * 	Matthew Jakeman <mjakeman26@outlook.co.nz>
 */

#pragma once

#include <glib-object.h>
#include <lsmdom.h>

G_BEGIN_DECLS

#define LSM_TYPE_DOM_CURSOR (lsm_dom_cursor_get_type())

G_DECLARE_FINAL_TYPE (LsmDomCursor, lsm_dom_cursor, LSM, DOM_CURSOR, GObject)

typedef enum
{
    LSM_DIRECTION_UP,
    LSM_DIRECTION_DOWN,
    LSM_DIRECTION_LEFT,
    LSM_DIRECTION_RIGHT,
} LsmDirection;

LsmDomCursor *lsm_dom_cursor_new (void);

void lsm_dom_cursor_move (LsmDomCursor *self, LsmDirection direction);

G_END_DECLS
