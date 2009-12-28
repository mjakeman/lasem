/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#ifndef LSM_MATHML_ATTRIBUTES_H
#define LSM_MATHML_ATTRIBUTES_H

#include <lsmattributes.h>
#include <lsmdom.h>
#include <lsmmathml.h>
#include <lsmmathmlenums.h>
#include <lsmmathmltraits.h>

G_BEGIN_DECLS

typedef struct {
	LsmAttribute base;
	gboolean value;
} LsmMathmlBooleanAttribute;

typedef struct {
	LsmAttribute base;
	unsigned int value;
} LsmMathmlUnsignedAttribute;

typedef struct {
	LsmAttribute base;
	unsigned int value;
} LsmMathmlEnumAttribute;

typedef struct {
	LsmAttribute base;
	double value;
} LsmMathmlDoubleAttribute;

typedef struct {
	LsmAttribute base;
	LsmMathmlColor color;
} LsmMathmlColorAttribute;

typedef struct {
	LsmAttribute base;
	char *value;
} LsmMathmlStringAttribute;

gboolean		lsm_mathml_boolean_attribute_inherit 	(LsmMathmlBooleanAttribute *attribute,
								 gboolean value);
unsigned int		lsm_mathml_enum_attribute_inherit	(LsmMathmlEnumAttribute *attribute,
								 unsigned int value);
double 			lsm_mathml_double_attribute_inherit 	(LsmMathmlDoubleAttribute *attribute,
								 double value);
LsmMathmlColor		lsm_mathml_color_attribute_inherit 	(LsmMathmlColorAttribute *attribute,
								 LsmMathmlColor value);
const char *		lsm_mathml_string_attribute_inherit	(LsmMathmlStringAttribute *attribute,
								 const char *string);

typedef struct {
	LsmAttribute base;
	LsmMathmlLength length;
        double value;
} LsmMathmlLengthAttribute;

double 		lsm_mathml_length_attribute_normalize	(LsmMathmlLengthAttribute *atribute, double default_value,
							 double font_size);

/*******************************/

typedef struct {
	char *value;
	char *css_value;
	LsmMathmlCssType css_type;
} LsmMathmlAttribute;

typedef struct {
	GHashTable *attribute_hash;
	GHashTable *bag_hash;
} LsmMathmlAttributeMap;

typedef struct {
} LsmMathmlAttributeBag;

typedef struct {
	void * 			(*init) 	(void);
	void 			(*finalize) 	(void *bag);
} LsmMathmlAttributeBagClass;

typedef struct {
	void (*finalize) (void *attribute);
} LsmMathmlAttributeClass;

typedef void (*LsmMathmlAttributeFinalizeFunc) (void *);

LsmMathmlAttributeMap *	lsm_mathml_attribute_map_new 		(void);
LsmMathmlAttributeMap *	lsm_mathml_attribute_map_duplicate		(const LsmMathmlAttributeMap *from);
void			lsm_mathml_attribute_map_free		(LsmMathmlAttributeMap *map);

void 		lsm_mathml_attribute_map_add_bag_attribute  	(LsmMathmlAttributeMap *map,
								 const char *name,
								 ptrdiff_t attribute_offset,
								 const LsmMathmlAttributeClass *attribute_class,
								 ptrdiff_t bag_offset,
								 const LsmMathmlAttributeBagClass *bag_class);
void		lsm_mathml_attribute_map_add_attribute_full	(LsmMathmlAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset,
								 const LsmMathmlAttributeClass *attribute_class);
void		lsm_mathml_attribute_map_add_attribute 		(LsmMathmlAttributeMap *map,
								 char const *name,
								 ptrdiff_t offset);

void		lsm_mathml_attribute_map_free_attributes 	(LsmMathmlAttributeMap *map,
								 void *instance);

gboolean	lsm_mathml_attribute_map_set_attribute		(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value);
char const *	lsm_mathml_attribute_map_get_attribute		(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name);
gboolean	lsm_mathml_attribute_map_set_css_attribute	(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name,
								 char const *value,
								 LsmMathmlCssType css_type);
gboolean	lsm_mathml_attribute_map_is_attribute_defined	(LsmMathmlAttributeMap *map,
								 void *instance,
								 char const *name);

gboolean 	lsm_mathml_attribute_is_defined 		(const LsmMathmlAttribute *attribute);
char const * 	lsm_mathml_attribute_get_value 			(const LsmMathmlAttribute *attribute);

typedef unsigned int (*LsmDomNamedConvert) (const char *string);

typedef struct {
	unsigned int n_values;
	unsigned int *values;
} LsmMathmlEnumList;

typedef struct {
	LsmMathmlAttribute attr;
	unsigned int n_values;
	unsigned int *values;
} LsmMathmlEnumListAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	int value;
} LsmMathmlScriptLevelAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlSpace space;
	double value;
} LsmMathmlSpaceAttribute;

typedef struct {
	LsmMathmlAttribute attr;
	LsmMathmlSpaceList *space_list;
	double *values;
} LsmMathmlSpaceListAttribute;

void 		lsm_mathml_enum_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value,
							 LsmDomNamedConvert convert);

void		lsm_mathml_string_attribute_finalize	(void *abstract);
void 		lsm_mathml_enum_list_attribute_finalize	(void *abstract);


void 		lsm_mathml_attribute_map_add_enum_list 	(LsmMathmlAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);

void 		lsm_mathml_script_level_attribute_parse	(LsmMathmlScriptLevelAttribute *attribute,
							 int *default_value);
void 		lsm_mathml_space_attribute_parse 	(LsmMathmlSpaceAttribute *attribute,
							 LsmMathmlSpace *style_value,
							 LsmMathmlStyle *style);

void 		lsm_mathml_space_list_attribute_parse 	(LsmMathmlSpaceListAttribute *attribute,
							 LsmMathmlSpaceList *style_value,
							 const LsmMathmlStyle *style);

void 	lsm_mathml_row_align_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);
void 	lsm_mathml_column_align_list_attribute_parse 	(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);
void 	lsm_mathml_line_list_attribute_parse 		(LsmMathmlEnumListAttribute *attribute,
							 LsmMathmlEnumList *style_value);

void 	lsm_mathml_attribute_map_add_space_list 	(LsmMathmlAttributeMap *map,
							 char const *name,
							 ptrdiff_t offset);
G_END_DECLS

#endif
