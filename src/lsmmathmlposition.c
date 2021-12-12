#include "lsmmathmlposition.h"

// TODO: Should this be a GObject or a struct?
// Think about language bindings

struct _LsmMathmlPosition
{
    GObject parent_instance;
    LsmMathmlElement *parent;
    int position;
};

G_DEFINE_FINAL_TYPE (LsmMathmlPosition, lsm_mathml_position, G_TYPE_OBJECT)

enum {
    PROP_0,
    PROP_PARENT,
    PROP_POSITION,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

LsmMathmlPosition *
lsm_mathml_position_new (LsmMathmlElement *parent, int position)
{
    return g_object_new (LSM_TYPE_MATHML_POSITION,
                         "parent", parent,
                         "position", position,
                         NULL);
}

static void
lsm_mathml_position_finalize (GObject *object)
{
    LsmMathmlPosition *self = (LsmMathmlPosition *)object;

    G_OBJECT_CLASS (lsm_mathml_position_parent_class)->finalize (object);
}

static void
lsm_mathml_position_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    LsmMathmlPosition *self = LSM_MATHML_POSITION (object);

    switch (prop_id)
    {
    case PROP_PARENT:
        g_value_set_object (value, self->parent);
        break;
    case PROP_POSITION:
        g_value_set_int (value, self->position);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
lsm_mathml_position_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    LsmMathmlPosition *self = LSM_MATHML_POSITION (object);

    switch (prop_id)
    {
    case PROP_PARENT:
        self->parent = g_value_get_object (value);
        break;
    case PROP_POSITION:
        self->position = g_value_get_int (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
lsm_mathml_position_class_init (LsmMathmlPositionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = lsm_mathml_position_finalize;
    object_class->get_property = lsm_mathml_position_get_property;
    object_class->set_property = lsm_mathml_position_set_property;

    properties [PROP_PARENT]
        = g_param_spec_object ("parent", "Parent", "Parent", LSM_TYPE_MATHML_ELEMENT, G_PARAM_READWRITE);

    properties [PROP_POSITION]
        = g_param_spec_int ("position", "Position", "Position", -1, G_MAXINT, 0, G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
lsm_mathml_position_init (LsmMathmlPosition *self)
{
}
