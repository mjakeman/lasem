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

#include "lsmmathmlcursor.h"
#include "lsmmathmlposition.h"

#include <math.h>

struct _LsmMathmlCursor
{
    GObject parent_instance;

    LsmMathmlElement *current;

    // Currently 0=start, 1=end
    int position;
};

G_DEFINE_FINAL_TYPE (LsmMathmlCursor, lsm_mathml_cursor, G_TYPE_OBJECT)

enum {
    PROP_0,
    PROP_CURRENT,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

LsmMathmlCursor *
lsm_mathml_cursor_new (void)
{
    return g_object_new (LSM_TYPE_MATHML_CURSOR, NULL);
}

static void
lsm_mathml_cursor_finalize (GObject *object)
{
    LsmMathmlCursor *self = (LsmMathmlCursor *)object;

    G_OBJECT_CLASS (lsm_mathml_cursor_parent_class)->finalize (object);
}

static void
lsm_mathml_cursor_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    LsmMathmlCursor *self = LSM_MATHML_CURSOR (object);

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
lsm_mathml_cursor_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    LsmMathmlCursor *self = LSM_MATHML_CURSOR (object);

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

static LsmMathmlPosition *
do_move_recursive (LsmMathmlCursor *self, LsmDirection direction)
{
    int new_index = -1;
    LsmDomNode *element = LSM_DOM_NODE (self->current);

    // TODO: Won't work, as self->position is only valid for current position in tree (i.e. will
    // be different position for parents)     ...what now?
    while (!(LSM_MATHML_ELEMENT_CLASS (element)->get_next_position(LSM_MATHML_ELEMENT (element), self->position, direction, &new_index)))
    {
        element = lsm_dom_node_get_parent_node (LSM_DOM_NODE (element));
        if (!element)
            return NULL;
    }

    // TODO: Once an element accepts the movement, go to its "first" position
    return lsm_mathml_position_new (LSM_MATHML_ELEMENT (element), new_index);
}

static void
get_points_recursive (LsmMathmlElement *element, GSList **points)
{
    LsmMathmlElement *next;

    // Add points (for now, two per element)
    if (!LSM_IS_MATHML_ROW_ELEMENT (element)) // "Invisible"
    {
        // TODO: Coalesce insertion points if they are within a parent row element
        // We want to collapse the right cursor, e.g. the cursor "belongs" to the element
        // immediately to the right of it - the '0' position cursor always wins.
        *points = g_slist_append (*points, lsm_mathml_position_new (element, 0));
        *points = g_slist_append (*points, lsm_mathml_position_new (element, 1));
    }

    // Go to children elements first
    next = lsm_dom_node_get_first_child (element);

    if (next && LSM_IS_MATHML_ELEMENT (next))
        get_points_recursive (next, points);

    // Then move to sibling elements
    next = lsm_dom_node_get_next_sibling (element);

    if (next && LSM_IS_MATHML_ELEMENT (next))
        get_points_recursive (next, points);
}

// TODO: Move to some kind of editor object?
GSList *
lsm_mathml_cursor_get_insertion_points (LsmMathmlElement *root)
{
    GSList *points = NULL;

    // DFS traverse over element
    // get all insertion points
    get_points_recursive (root, &points);

    // later:
    // loop over insertion points
    // find closest rectilinear distance
    /*for (GSList *l = points; l != NULL; l = l->next)
    {
        LsmMathmlPosition *pos = LSM_MATHML_POSITION (l->data);

        int position;
        LsmMathmlElement *parent;
        g_object_get (pos, "parent", &parent, "position", &position, NULL);
        g_print ("Insertion Point: %s %d\n",
                 lsm_dom_node_get_node_name (LSM_DOM_NODE (parent)),
                 position);
    }*/

    return points;
}

LsmMathmlPosition *
lsm_mathml_cursor_get_nearest_insertion_point (LsmMathmlElement *root, double x, double y)
{
    // TODO: The whole x/y doesn't start at zero is quite confusing for
    // implementors. For now, we'll translate x/y to account for the baseline
    // but a better solution is needed long term.

    GSList *points = lsm_mathml_cursor_get_insertion_points (root);

    double min_distance = G_MAXDOUBLE;
    LsmMathmlPosition *min_point = NULL;

    for (GSList *l = points; l != NULL; l = l->next)
    {
        LsmMathmlPosition *pos = LSM_MATHML_POSITION (l->data);

        int position;
        LsmMathmlElement *parent;
        g_object_get (pos, "parent", &parent, "position", &position, NULL);

        // TODO: Make elements define their own insertion points
        // (otherwise defeats the purpose)

        // TODO: We should find the shortest distance between two
        // lines. That's a bit hard, so we'll find the midpoint distance
        // for now.

        double midpoint_x, midpoint_y;

        if (position == 0)
        {
            midpoint_x = parent->x;
        }
        else
        {
            g_assert (position == 1);

            midpoint_x = parent->x + parent->bbox.width;
        }

        midpoint_y = ((parent->y - parent->bbox.height) + (parent->y + parent->bbox.depth)) / 2;

        double distance = sqrt(pow(x - midpoint_x, 2) + pow(y - midpoint_y, 2));

        if (distance < min_distance)
        {
            min_distance = distance;
            min_point = lsm_mathml_position_new (parent, position);
        }
    }

    if (min_point == NULL)
    {
        g_critical ("No insertion point found!\n");
        return NULL;
    }

    int position;
    LsmMathmlElement *parent;
    g_object_get (min_point, "parent", &parent, "position", &position, NULL);
    g_print ("Nearest Insertion Point: %s %d (dist %lf)\n",
             lsm_dom_node_get_node_name (LSM_DOM_NODE (parent)),
             position,
             min_distance);

    g_object_ref (min_point);

    g_slist_free_full (points, g_object_unref);

    return min_point;
}

void
lsm_mathml_cursor_move (LsmMathmlCursor *self, LsmDirection direction)
{
    LsmMathmlElement *move_to = NULL;
    int new_pos = self->position;

    if (!self->current)
        return;

    /*LsmMathmlPosition *test = do_move_recursive (self, direction);
    if (test)
    {
        int position;
        LsmMathmlElement *parent;
        g_object_get (test, "position", &position, "parent", &parent, NULL);
        g_print ("Result of Move: Element '%s' at index '%d'",
                 lsm_dom_node_get_node_name (parent), position);
    }*/

    if (direction == LSM_DIRECTION_LEFT)
    {
        // TODO: First try move within the current element

        if (self->position != 0)
        {
            // At end, try to move to a child element
            move_to = lsm_dom_node_get_last_child (self->current);
            new_pos = 1;

            if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
                goto move;
        }

        // Failing that, move to the previous sibling node
        move_to = lsm_dom_node_get_previous_sibling (self->current);
        new_pos = 1;

        if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
            goto move;

        // Move to parent start
        move_to = lsm_dom_node_get_parent_node (self->current);
        new_pos = 0;

        if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
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

            if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
                goto move;
        }

        // Try move to next sibling
        move_to = lsm_dom_node_get_next_sibling (self->current);
        new_pos = 0;

        if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
            goto move;

        // Failing that, move to parent end
        move_to = lsm_dom_node_get_parent_node (self->current);
        new_pos = 1;

        if (move_to && LSM_IS_MATHML_ELEMENT (move_to))
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
lsm_mathml_cursor_class_init (LsmMathmlCursorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = lsm_mathml_cursor_finalize;
    object_class->get_property = lsm_mathml_cursor_get_property;
    object_class->set_property = lsm_mathml_cursor_set_property;

    properties [PROP_CURRENT]
        = g_param_spec_object ("current",
                               "Current",
                               "Currently selected element",
                               LSM_TYPE_MATHML_ELEMENT,
                               G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
lsm_mathml_cursor_init (LsmMathmlCursor *self)
{
}
