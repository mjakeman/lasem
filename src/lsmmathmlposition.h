#pragma once

#include <glib-object.h>

#include "lsmmathml.h"

G_BEGIN_DECLS

#define LSM_TYPE_MATHML_POSITION (lsm_mathml_position_get_type())

G_DECLARE_FINAL_TYPE (LsmMathmlPosition, lsm_mathml_position, LSM, MATHML_POSITION, GObject)

LsmMathmlPosition *lsm_mathml_position_new (LsmMathmlElement *parent, int position);

G_END_DECLS
