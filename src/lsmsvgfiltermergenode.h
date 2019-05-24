/* Lasem
 *
 * Copyright © 2012 Emmanuel Pacaud
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
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_SVG_FILTER_MERGE_NODE_H
#define LSM_SVG_FILTER_MERGE_NODE_H

#include <lsmsvgtypes.h>
#include <lsmsvgfilterprimitive.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_FILTER_MERGE_NODE             (lsm_svg_filter_merge_node_get_type ())
#define LSM_SVG_FILTER_MERGE_NODE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_FILTER_MERGE_NODE, LsmSvgFilterMergeNode))
#define LSM_SVG_FILTER_MERGE_NODE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_FILTER_MERGE_NODE, LsmSvgFilterMergeNodeClass))
#define LSM_IS_SVG_FILTER_MERGE_NODE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_FILTER_MERGE_NODE))
#define LSM_IS_SVG_FILTER_MERGE_NODE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_FILTER_MERGE_NODE))
#define LSM_SVG_FILTER_MERGE_NODE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_FILTER_MERGE_NODE, LsmSvgFilterMergeNodeClass))

typedef struct _LsmSvgFilterMergeNodeClass LsmSvgFilterMergeNodeClass;

struct _LsmSvgFilterMergeNode {
	LsmSvgFilterPrimitive base;

	LsmSvgOneOrTwoDoubleAttribute std_deviation;
};

struct _LsmSvgFilterMergeNodeClass {
	LsmSvgFilterPrimitiveClass  element_class;
};

GType lsm_svg_filter_merge_node_get_type (void);

LsmDomNode * 	lsm_svg_filter_merge_node_new 		(void);

G_END_DECLS

#endif
