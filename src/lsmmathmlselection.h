#pragma once

#include <glib-object.h>
#include <lsmmathml.h>

G_BEGIN_DECLS

#define LSM_TYPE_MATHML_SELECTION (lsm_mathml_selection_get_type())

G_DECLARE_FINAL_TYPE (LsmMathmlSelection, lsm_mathml_selection, LSM, MATHML_SELECTION, GObject)

LsmMathmlSelection *lsm_mathml_selection_new (void);

LsmMathmlElement *
lsm_mathml_selection_get_selection_bounds (LsmMathmlSelection *selection,
                                           LsmMathmlBbox      *out_bbox);

G_END_DECLS
