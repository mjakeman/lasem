/* Lasem
 *
 * Copyright © 2009 Emmanuel Pacaud
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

#include <lsmdebug.h>
#include <lsmsvgaelement.h>
#include <lsmsvgcircleelement.h>
#include <lsmsvgclippathelement.h>
#include <lsmsvgdefselement.h>
#include <lsmsvgdocument.h>
#include <lsmsvgellipseelement.h>
#include <lsmsvgfilterelement.h>
#include <lsmsvgfilterblend.h>
#include <lsmsvgfiltercolormatrix.h>
#include <lsmsvgfiltercomposite.h>
#include <lsmsvgfilterconvolvematrix.h>
#include <lsmsvgfilterdisplacementmap.h>
#include <lsmsvgfilterflood.h>
#include <lsmsvgfiltergaussianblur.h>
#include <lsmsvgfilterimage.h>
#include <lsmsvgfilteroffset.h>
#include <lsmsvgfiltermerge.h>
#include <lsmsvgfiltermergenode.h>
#include <lsmsvgfiltermorphology.h>
#include <lsmsvgfilterspecularlighting.h>
#include <lsmsvgfilterturbulence.h>
#include <lsmsvgfiltertile.h>
#include <lsmsvggelement.h>
#include <lsmsvgimageelement.h>
#include <lsmsvglineargradientelement.h>
#include <lsmsvglineelement.h>
#include <lsmsvgmarkerelement.h>
#include <lsmsvgmaskelement.h>
#include <lsmsvgpolylineelement.h>
#include <lsmsvgpolygonelement.h>
#include <lsmsvgpathelement.h>
#include <lsmsvgpatternelement.h>
#include <lsmsvgradialgradientelement.h>
#include <lsmsvgrectelement.h>
#include <lsmsvgsvgelement.h>
#include <lsmsvgstopelement.h>
#include <lsmsvgswitchelement.h>
#include <lsmsvgsymbolelement.h>
#include <lsmsvgtextelement.h>
#include <lsmsvgtspanelement.h>
#include <lsmsvguseelement.h>
#include <lsmsvgview.h>
#include <string.h>

static GObjectClass *parent_class;

/* LsmDomNode implementation */

static gboolean
lsm_svg_document_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return (LSM_IS_SVG_SVG_ELEMENT (child));
}

/* LsmDomDocument implementation */

LsmSvgElement *
lsm_svg_document_get_element_by_url (LsmSvgDocument *document, const char *url)
{
	LsmSvgElement *element;
	char *end;
	char *id;

	g_return_val_if_fail (LSM_IS_SVG_DOCUMENT (document), NULL);

	if (url == NULL || strncmp (url, "url(#", 5) != 0)
		return NULL;

	id = g_strdup (url + 5);
	for (end = id; *end != '\0' && *end != ')'; end++);
	*end = '\0';

	element = lsm_svg_document_get_element_by_id (document, id);

	g_free (id);

	return element;
}

static LsmDomElement *
_create_element (LsmDomDocument *document, const char *tag_name)
{
	LsmDomNode *node = NULL;

	if (strcmp (tag_name, "svg") == 0)
		node = lsm_svg_svg_element_new ();
	else if (strcmp (tag_name, "g") == 0)
		node = lsm_svg_g_element_new ();
	else if (strcmp (tag_name, "rect") == 0)
		node = lsm_svg_rect_element_new ();
	else if (strcmp (tag_name, "circle") == 0)
		node = lsm_svg_circle_element_new ();
	else if (strcmp (tag_name, "ellipse") == 0)
		node = lsm_svg_ellipse_element_new ();
	else if (strcmp (tag_name, "path") == 0)
		node = lsm_svg_path_element_new ();
	else if (strcmp (tag_name, "line") == 0)
		node = lsm_svg_line_element_new ();
	else if (strcmp (tag_name, "polyline") == 0)
		node = lsm_svg_polyline_element_new ();
	else if (strcmp (tag_name, "polygon") == 0)
		node = lsm_svg_polygon_element_new ();
	else if (strcmp (tag_name, "text") == 0)
		node = lsm_svg_text_element_new ();
	else if (strcmp (tag_name, "tspan") == 0)
		node = lsm_svg_tspan_element_new ();
	else if (strcmp (tag_name, "linearGradient") == 0)
		node = lsm_svg_linear_gradient_element_new ();
	else if (strcmp (tag_name, "radialGradient") == 0)
		node = lsm_svg_radial_gradient_element_new ();
	else if (strcmp (tag_name, "stop") == 0)
		node = lsm_svg_stop_element_new ();
	else if (strcmp (tag_name, "pattern") == 0)
		node = lsm_svg_pattern_element_new ();
	else if (strcmp (tag_name, "mask") == 0)
		node = lsm_svg_mask_element_new ();
	else if (strcmp (tag_name, "use") == 0)
		node = lsm_svg_use_element_new ();
	else if (strcmp (tag_name, "image") == 0)
		node = lsm_svg_image_element_new ();
	else if (strcmp (tag_name, "defs") == 0)
		node = lsm_svg_defs_element_new ();
	else if (strcmp (tag_name, "symbol") == 0)
		node = lsm_svg_symbol_element_new ();
	else if (strcmp (tag_name, "marker") == 0)
		node = lsm_svg_marker_element_new ();
	else if (strcmp (tag_name, "clipPath") == 0)
		node = lsm_svg_clip_path_element_new ();
	else if (strcmp (tag_name, "switch") == 0)
		node = lsm_svg_switch_element_new ();
	else if (strcmp (tag_name, "a") == 0)
		node = lsm_svg_a_element_new ();
	else if (strcmp (tag_name, "filter") == 0)
		node = lsm_svg_filter_element_new ();
	else if (strcmp (tag_name, "feBlend") == 0)
		node = lsm_svg_filter_blend_new ();
	else if (strcmp (tag_name, "feComposite") == 0)
		node = lsm_svg_filter_composite_new ();
	else if (strcmp (tag_name, "feColorMatrix") == 0)
		node = lsm_svg_filter_color_matrix_new ();
	else if (strcmp (tag_name, "feConvolveMatrix") == 0)
		node = lsm_svg_filter_convolve_matrix_new ();
	else if (strcmp (tag_name, "feDisplacementMap") == 0)
		node = lsm_svg_filter_displacement_map_new ();
	else if (strcmp (tag_name, "feFlood") == 0)
		node = lsm_svg_filter_flood_new ();
	else if (strcmp (tag_name, "feGaussianBlur") == 0)
		node = lsm_svg_filter_gaussian_blur_new ();
	else if (strcmp (tag_name, "feImage") == 0)
		node = lsm_svg_filter_image_new ();
	else if (strcmp (tag_name, "feMerge") == 0)
		node = lsm_svg_filter_merge_new ();
	else if (strcmp (tag_name, "feMergeNode") == 0)
		node = lsm_svg_filter_merge_node_new ();
	else if (strcmp (tag_name, "feMorphology") == 0)
		node = lsm_svg_filter_morphology_new ();
	else if (strcmp (tag_name, "feOffset") == 0)
		node = lsm_svg_filter_offset_new ();
	else if (strcmp (tag_name, "feSpecularLighting") == 0)
		node = lsm_svg_filter_specular_lighting_new ();
	else if (strcmp (tag_name, "feTile") == 0)
		node = lsm_svg_filter_tile_new ();
	else if (strcmp (tag_name, "feTurbulence") == 0)
		node = lsm_svg_filter_turbulence_new ();

	if (node != NULL)
		lsm_debug_dom ("[LsmSvgDocument::create_element] Create a %s element", tag_name);

	return LSM_DOM_ELEMENT (node);
}

static LsmDomElement *
lsm_svg_document_create_element (LsmDomDocument *document, const char *tag_name)
{
	LsmDomElement *element;

	element = _create_element (document, tag_name);
	if (element != NULL)
		return element;

	if (g_str_has_prefix (tag_name, "svg:"))
		return _create_element (document, &tag_name[4]);

	return NULL;
}

static LsmDomView *
lsm_svg_document_create_view (LsmDomDocument *document)
{
	return LSM_DOM_VIEW (lsm_svg_view_new (LSM_SVG_DOCUMENT (document)));
}

/* LsmSvgDocument implementation */

/**
 * lsm_svg_document_get_element_by_id:
 * @self: a #LsmSvgDocument
 * @id: id of the element to find
 *
 * Returns: (transfer none): the requested element, NULL if not found.
 */

LsmSvgElement *
lsm_svg_document_get_element_by_id (LsmSvgDocument *self, const char *id)
{
	g_return_val_if_fail (LSM_IS_SVG_DOCUMENT (self), NULL);
	g_return_val_if_fail (id != NULL, NULL);

	lsm_debug_dom ("[LsmSvgDocument::get_element_by_id] Lookup '%s'", id);

	return g_hash_table_lookup (self->ids, id);
}

void
lsm_svg_document_register_element (LsmSvgDocument *self, LsmSvgElement *element, const char *id, const char *old_id)
{
	g_return_if_fail (LSM_IS_SVG_DOCUMENT (self));
	g_return_if_fail (LSM_IS_SVG_ELEMENT (element));

	if (old_id != NULL) {
		lsm_debug_dom ("[LsmSvgDocument::register_element] Unregister '%s'", old_id);

		g_hash_table_remove (self->ids, old_id);
	}

	if (id != NULL) {
		char *new_id = g_strdup (id);

		lsm_debug_dom ("[LsmSvgDocument::register_element] Register '%s'", id);

		g_hash_table_replace (self->ids, new_id, element);
	}
}

LsmDomDocument *
lsm_svg_document_new (void)
{
	return g_object_new (LSM_TYPE_SVG_DOCUMENT, NULL);
}

static void
lsm_svg_document_init (LsmSvgDocument *document)
{
	document->ids = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
}

static void
lsm_svg_document_finalize (GObject *object)
{
	LsmSvgDocument *document = LSM_SVG_DOCUMENT (object);

	g_hash_table_unref (document->ids);

	parent_class->finalize (object);
}

LsmSvgSvgElement *
lsm_svg_document_get_root_element (const LsmSvgDocument *document)
{
	return LSM_SVG_SVG_ELEMENT (lsm_dom_document_get_document_element (LSM_DOM_DOCUMENT (document)));
}

/* LsmSvgDocument class */

static void
lsm_svg_document_class_init (LsmSvgDocumentClass *this_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (this_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (this_class);
	LsmDomDocumentClass *d_document_class = LSM_DOM_DOCUMENT_CLASS (this_class);

	parent_class = g_type_class_peek_parent (this_class);

	object_class->finalize = lsm_svg_document_finalize;

	d_node_class->can_append_child = lsm_svg_document_can_append_child;

	d_document_class->create_element = lsm_svg_document_create_element;
	d_document_class->create_view = lsm_svg_document_create_view;
}

G_DEFINE_TYPE (LsmSvgDocument, lsm_svg_document, LSM_TYPE_DOM_DOCUMENT)
