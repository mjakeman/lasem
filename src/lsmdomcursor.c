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

#include "lsmdomcursor.h"

struct _LsmDomCursor
{
    GObject parent_instance;

    LsmDomNode *current;

    // Currently 0=start, 1=end
    int position;
};

G_DEFINE_FINAL_TYPE (LsmDomCursor, lsm_dom_cursor, G_TYPE_OBJECT)

enum {
    PROP_0,
    PROP_CURRENT,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

LsmDomCursor *
lsm_dom_cursor_new (void)
{
    return g_object_new (LSM_TYPE_DOM_CURSOR, NULL);
}

static void
lsm_dom_cursor_finalize (GObject *object)
{
    LsmDomCursor *self = (LsmDomCursor *)object;

    G_OBJECT_CLASS (lsm_dom_cursor_parent_class)->finalize (object);
}

static void
lsm_dom_cursor_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    LsmDomCursor *self = LSM_DOM_CURSOR (object);

    switch (prop_id)
    {
    case PROP_CURRENT:
        g_value_set_object (value, self->current);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
lsm_dom_cursor_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    LsmDomCursor *self = LSM_DOM_CURSOR (object);

    switch (prop_id)
    {
    case PROP_CURRENT:
        self->current = g_value_get_object (value);
        self->position = 0;
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

void
lsm_dom_cursor_move (LsmDomCursor *self, LsmDirection direction)
{
    LsmDomNode *move_to = NULL;
    int new_pos = self->position;

    if (!self->current)
        return;

    if (direction == LSM_DIRECTION_LEFT)
    {
        // TODO: First try move within the current element

        if (self->position != 0)
        {
            // At end, try to move to a child element
            move_to = lsm_dom_node_get_last_child (self->current);
            new_pos = 1;

            if (move_to)
                goto move;
        }

        // Failing that, move to the previous sibling node
        move_to = lsm_dom_node_get_previous_sibling (self->current);
        new_pos = 1;

        if (move_to)
            goto move;

        // Move to parent start
        move_to = lsm_dom_node_get_parent_node (self->current);
        new_pos = 0;

        if (move_to)
            goto move;
    }
    else if (direction == LSM_DIRECTION_RIGHT)
    {
        // Try move within the current element
        // TODO HERE

        if (self->position != 1)
        {
            // At start, try to move to a child element
            move_to = lsm_dom_node_get_first_child (self->current);
            new_pos = 0;

            if (move_to)
                goto move;
        }

        // Try move to next sibling
        move_to = lsm_dom_node_get_next_sibling (self->current);
        new_pos = 0;

        if (move_to)
            goto move;

        // Failing that, move to parent end
        move_to = lsm_dom_node_get_parent_node (self->current);
        new_pos = 1;

        if (move_to)
            goto move;
    }

    return;

move:
    // TODO: Emit notify?
    self->current = move_to;
    self->position = new_pos;
    g_print ("Moved to: %s (%d)\n", lsm_dom_node_get_node_name (self->current), self->position);
}

static void
lsm_dom_cursor_class_init (LsmDomCursorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = lsm_dom_cursor_finalize;
    object_class->get_property = lsm_dom_cursor_get_property;
    object_class->set_property = lsm_dom_cursor_set_property;

    properties [PROP_CURRENT]
        = g_param_spec_object ("current",
                               "Current",
                               "Currently selected element",
                               LSM_TYPE_DOM_NODE,
                               G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
lsm_dom_cursor_init (LsmDomCursor *self)
{
}
