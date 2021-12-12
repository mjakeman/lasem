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

    LsmMathmlPosition *position;
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
        g_value_set_object (value, self->position);
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
        self->position = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
get_points_recursive (LsmMathmlElement *element, GList **points)
{
    LsmMathmlElement *next;

    // Add points (for now, two per element)
    if (!LSM_IS_MATHML_ROW_ELEMENT (element) &&
        !LSM_IS_MATHML_MATH_ELEMENT (element) && // Layout Elements
        !LSM_IS_MATHML_SCRIPT_ELEMENT (element)) // Scripts do not use first caret location
    {
        *points = g_list_append (*points, lsm_mathml_position_new (element, 0));
    }

    // Go to children elements first
    next = lsm_dom_node_get_first_child (element);

    if (next && LSM_IS_MATHML_ELEMENT (next))
        get_points_recursive (next, points);

    // Add end point using same criteria as above
    if (!LSM_IS_MATHML_ROW_ELEMENT (element) && !LSM_IS_MATHML_MATH_ELEMENT (element)) // "Invisibles"
    {
        // Coalesce insertion points if they are within a parent row element
        // We want to collapse the left cursor, e.g. the cursor "belongs" to the element
        // immediately to the left of it - the '0' position cursor of the right-hand element
        // always wins.
        LsmDomNode *test_sibling = lsm_dom_node_get_next_sibling (element);
        LsmDomNode *test_parent = lsm_dom_node_get_parent_node (element);

        if (!(test_sibling != NULL && LSM_IS_MATHML_ROW_ELEMENT (test_parent)))
        {
            *points = g_list_append (*points, lsm_mathml_position_new (element, 1));
        }
    }

    // Then move to sibling elements
    next = lsm_dom_node_get_next_sibling (element);

    if (next && LSM_IS_MATHML_ELEMENT (next))
        get_points_recursive (next, points);
}

// TODO: Move to some kind of editor object?
GList *
lsm_mathml_cursor_get_insertion_points (LsmMathmlElement *root)
{
    GList *points = NULL;

    // DFS traverse over element and collect all insertion points
    get_points_recursive (root, &points);

    return points;
}

static double
min_distance_point_to_line (double px, double py, double lx, double ly1, double ly2)
{
    // ensure ly1 is always less than ly2
    if (ly1 > ly2)
    {
        double temp = ly1;
        ly1 = ly2;
        ly2 = temp;
    }

    double distance;

    // CASE 1: Point above Line
    if (py < ly1)
    {
        distance = sqrt(pow(px - lx, 2) + pow(py - ly1, 2));
    }

    // CASE 2: Point adjacent to Line
    else if (py > ly1 && py < ly2)
    {
        // Find perpendicular distance
        distance = px - lx;
    }

    // CASE 3: Point below Line
    else
    {
        distance = sqrt(pow(px - lx, 2) + pow(py - ly2, 2));
    }

    // Return absolute value
    return fabs (distance);
}

LsmMathmlPosition *
lsm_mathml_cursor_get_nearest_insertion_point (LsmMathmlElement *root, double x, double y)
{
    // TODO: The whole x/y doesn't start at zero is quite confusing for
    // implementors. For now, we'll translate x/y to account for the baseline
    // but a better solution is needed long term.

    GList *points = lsm_mathml_cursor_get_insertion_points (root);

    double min_distance = G_MAXDOUBLE;
    LsmMathmlPosition *min_point = NULL;

    for (GList *l = points; l != NULL; l = l->next)
    {
        LsmMathmlPosition *pos = LSM_MATHML_POSITION (l->data);

        int position;
        LsmMathmlElement *parent;
        g_object_get (pos, "parent", &parent, "position", &position, NULL);

        // TODO: Make elements define their own insertion points
        // (otherwise defeats the purpose)

        double midpoint_x;

        if (position == 0)
        {
            midpoint_x = parent->x;
        }
        else
        {
            g_assert (position == 1);

            midpoint_x = parent->x + parent->bbox.width;
        }

        double distance = min_distance_point_to_line (x, y, midpoint_x, parent->y - parent->bbox.height, parent->y + parent->bbox.depth);

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

    g_list_free_full (points, g_object_unref);

    return min_point;
}

static int
position_cmp (gconstpointer a, gconstpointer b)
{
    // TODO: These being GObjects might be too expensive
    // Let's make them GBoxed instead...?
    LsmMathmlPosition *pos_a = LSM_MATHML_POSITION (a);
    LsmMathmlPosition *pos_b = LSM_MATHML_POSITION (b);

    int position_a, position_b;
    LsmMathmlElement *parent_a, *parent_b;
    g_object_get (pos_a, "parent", &parent_a, "position", &position_a, NULL);
    g_object_get (pos_b, "parent", &parent_b, "position", &position_b, NULL);

    if (position_a == position_b &&
        parent_a == parent_b)
        return 0;

    return 1;
}

void
lsm_mathml_cursor_move (LsmMathmlCursor *self, LsmMathmlElement *root, LsmDirection direction)
{
    GList *points = lsm_mathml_cursor_get_insertion_points (root);
    LsmMathmlPosition *current = self->position;

    if (current == NULL) {
        current = LSM_MATHML_POSITION (points->data);
        g_object_notify_by_pspec (self, properties[PROP_CURRENT]);
        return;
    }

    g_assert (current != NULL);

    GList *current_list = g_list_find_custom (points, current, position_cmp);

    g_assert (current_list != NULL);

    if (direction == LSM_DIRECTION_LEFT &&
        current_list->prev != NULL)
    {
        self->position = LSM_MATHML_POSITION (current_list->prev->data);
    }
    else if (direction == LSM_DIRECTION_RIGHT &&
         current_list->next != NULL)
    {
        self->position = LSM_MATHML_POSITION (current_list->next->data);
    }

    g_object_notify_by_pspec (self, properties[PROP_CURRENT]);
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
                               "Current cursor position",
                               LSM_TYPE_MATHML_POSITION,
                               G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
lsm_mathml_cursor_init (LsmMathmlCursor *self)
{
}
