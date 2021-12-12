#include "lsmmathmlselection.h"

#include "lsmmathmlposition.h"

struct _LsmMathmlSelection
{
    GObject parent_instance;

    // TODO: Convert to GBoxed?
    LsmMathmlPosition *start_pos;
    LsmMathmlPosition *end_pos;
};

G_DEFINE_FINAL_TYPE (LsmMathmlSelection, lsm_mathml_selection, G_TYPE_OBJECT)

enum {
    PROP_0,
    PROP_START_POSITION,
    PROP_END_POSITION,
    N_PROPS
};

static GParamSpec *properties [N_PROPS];

LsmMathmlSelection *
lsm_mathml_selection_new (void)
{
    return g_object_new (LSM_TYPE_MATHML_SELECTION, NULL);
}

static void
lsm_mathml_selection_finalize (GObject *object)
{
    LsmMathmlSelection *self = (LsmMathmlSelection *)object;

    G_OBJECT_CLASS (lsm_mathml_selection_parent_class)->finalize (object);
}

static void
lsm_mathml_selection_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
    LsmMathmlSelection *self = LSM_MATHML_SELECTION (object);

    switch (prop_id)
    {
    case PROP_START_POSITION:
        g_value_set_object (value, self->start_pos);
        break;
    case PROP_END_POSITION:
        g_value_set_object (value, self->end_pos);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
lsm_mathml_selection_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
    LsmMathmlSelection *self = LSM_MATHML_SELECTION (object);

    switch (prop_id)
    {
    case PROP_START_POSITION:
        self->start_pos = g_value_get_object (value);
        break;
    case PROP_END_POSITION:
        self->end_pos = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static LsmMathmlElement *
find_common_ancestor (LsmMathmlElement *first, LsmMathmlElement *second)
{
    GSList *ancestors = NULL;

    LsmDomNode *iter = LSM_DOM_NODE (first);
    while (iter != NULL)
    {
        LsmDomNode *parent = lsm_dom_node_get_parent_node (LSM_DOM_NODE (iter));
        ancestors = g_slist_append (ancestors, parent);
        iter = parent;
    }

    iter = LSM_DOM_NODE (second);
    while (iter != NULL)
    {
        LsmDomNode *parent = lsm_dom_node_get_parent_node (LSM_DOM_NODE (iter));
        GSList *test = g_slist_find (ancestors, parent);

        if (test && test->data)
        {
            // We don't support mathml elements parented by dom nodes (yet?)
            g_assert (LSM_IS_MATHML_ELEMENT (test->data));
            g_print ("%s\n", lsm_dom_node_get_node_name (test->data));
            return LSM_MATHML_ELEMENT (test->data);
        }

        iter = parent;
    }

    g_critical ("No common ancestor node found between '%s' and '%s'.",
                lsm_dom_node_get_node_name (LSM_DOM_NODE (first)),
                lsm_dom_node_get_node_name (LSM_DOM_NODE (second)));

    return NULL;
}

// Temporarily return common element (TODO: return bbox and x/y coords only)
LsmMathmlElement *
lsm_mathml_selection_get_selection_bounds (LsmMathmlSelection *selection,
                                           LsmMathmlBbox      *out_bbox)
{
    if (!selection->start_pos || !selection->end_pos || !out_bbox)
        return NULL;

    int start_pos, end_pos;
    LsmMathmlElement *start_el, *end_el;
    g_object_get (selection->start_pos, "position", &start_pos, "parent", &start_el, NULL);
    g_object_get (selection->end_pos, "position", &end_pos, "parent", &end_el, NULL);

    // TODO: Consider the case where start == end
    // TODO: Consider the case where start and end are siblings

    LsmMathmlElement *common = find_common_ancestor (start_el, end_el);
    const LsmMathmlBbox *bbox = lsm_mathml_element_get_bbox (common);
    memcpy (out_bbox, bbox, sizeof (LsmMathmlBbox));

    return common;
}

static void
lsm_mathml_selection_class_init (LsmMathmlSelectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = lsm_mathml_selection_finalize;
    object_class->get_property = lsm_mathml_selection_get_property;
    object_class->set_property = lsm_mathml_selection_set_property;

    properties [PROP_START_POSITION]
        = g_param_spec_object ("start-position",
                               "start-position",
                               "Start Position",
                               LSM_TYPE_MATHML_POSITION,
                               G_PARAM_READWRITE);

    properties [PROP_END_POSITION]
        = g_param_spec_object ("end-position",
                               "end-position",
                               "End Position",
                               LSM_TYPE_MATHML_POSITION,
                               G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
lsm_mathml_selection_init (LsmMathmlSelection *self)
{
}
