/* Lasem
 *
 * Copyright © 2010 Emmanuel Pacaud
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

#ifndef LSM_DOM_NODE_LIST_H
#define LSM_DOM_NODE_LIST_H

#include <lsmdomtypes.h>

G_BEGIN_DECLS

#define LSM_TYPE_DOM_NODE_LIST             (lsm_dom_node_list_get_type ())
#define LSM_DOM_NODE_LIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_DOM_NODE_LIST, LsmDomNodeList))
#define LSM_DOM_NODE_LIST_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_DOM_NODE_LIST, LsmDomNodeListClass))
#define LSM_IS_DOM_NODE_LIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_DOM_NODE_LIST))
#define LSM_IS_DOM_NODE_LIST_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_DOM_NODE_LIST))
#define LSM_DOM_NODE_LIST_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_DOM_NODE_LIST, LsmDomNodeListClass))

typedef struct _LsmDomNodeListClass LsmDomNodeListClass;

struct _LsmDomNodeList {
	GObject	object;
};

struct _LsmDomNodeListClass {
	GObjectClass parent_class;

	LsmDomNode *	(*get_item) 		(LsmDomNodeList *list, unsigned int index);
	unsigned int	(*get_length)		(LsmDomNodeList *list);
};

GType lsm_dom_node_list_get_type (void);

LsmDomNode *		lsm_dom_node_list_get_item 		(LsmDomNodeList *list, unsigned int index);
unsigned int		lsm_dom_node_list_get_length		(LsmDomNodeList *list);

G_END_DECLS

#endif
