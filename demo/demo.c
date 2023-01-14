#include <gtk/gtk.h>

#include <lsm.h>
#include <lsmdom.h>
#include <lsmmathml.h>

#include <lsmmathmlcursor.h>
#include <lsmmathmlselection.h>

#include <cairo.h>

// Subclass GTK Window so we can store demo state

#define DEMO_TYPE_WINDOW (demo_window_get_type())
G_DECLARE_FINAL_TYPE (DemoWindow, demo_window, DEMO, WINDOW, GtkApplicationWindow)

struct _DemoWindow
{
    GtkApplicationWindow parent_instance;

    GtkDrawingArea *lasem_view;
    GtkTextView *text_view;
    GtkLabel *status_label;
    GtkButton *render_btn;

    LsmDomDocument *document;
    GError *error;

    LsmDomView *view;
    double multiplier;

    LsmMathmlElement *pick;
    LsmMathmlCursor *cursor;

    LsmMathmlSelection *selection;
};

G_DEFINE_FINAL_TYPE (DemoWindow, demo_window, GTK_TYPE_APPLICATION_WINDOW)

/* see test.mml */
// static const char *mml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n  <mrow>\r\n    <msub>\r\n      <mi>a</mi>\r\n      <mn>0</mn>\r\n    </msub>\r\n    <mo>+</mo>\r\n    <mfrac>\r\n      <mn>1</mn>\r\n      <mrow>\r\n        <msub>\r\n          <mi>a</mi>\r\n          <mn>1</mn>\r\n        </msub>\r\n        <mo>+</mo>\r\n        <mfrac>\r\n          <mn>1</mn>\r\n          <mrow>\r\n            <msub>\r\n              <mi>a</mi>\r\n              <mn>2</mn>\r\n            </msub>\r\n            <mo>+</mo>\r\n            <mfrac>\r\n              <mn>1</mn>\r\n\r\n              <mrow>\r\n                <msub>\r\n                  <mi>a</mi>\r\n                  <mn>3</mn>\r\n                </msub>\r\n                <mo>+</mo>\r\n                <mfrac>\r\n                  <mn>1</mn>\r\n                  <mrow>\r\n                    <msub>\r\n                      <mi>a</mi>\r\n                      <mn>4</mn>\r\n                    </msub>\r\n                  </mrow>\r\n                </mfrac>\r\n              </mrow>\r\n\r\n            </mfrac>\r\n          </mrow>\r\n        </mfrac>\r\n      </mrow>\r\n    </mfrac>\r\n  </mrow>\r\n</math>\r\n\0";
static const char *mml = "<math xmlns = \"http://www.w3.org/1998/Math/MathML\">\n<mrow>\n<msup> <mi>x</mi> <mn>2</mn> </msup> <mo>+</mo>\n<mn>4</mn>\n<mi>x</mi>\n<mfrac>\n<mn>2</mn>\n<mi>y</mi>\n</mfrac>\n<mo>+</mo>\n<mn>4</mn>\n\n<mo>=</mo>\n<mn>0</mn>\n</mrow>\n</math>\0";

static GtkWidget *
demo_window_new (GtkApplication *app)
{
    return GTK_WIDGET (g_object_new (DEMO_TYPE_WINDOW,
                                     "application", app,
                                     NULL));
}

static void
demo_window_finalize (GObject *object)
{
    DemoWindow *self = (DemoWindow *)object;

    g_clear_pointer (&self->view, g_object_unref);
    g_clear_pointer (&self->document, g_object_unref);

    G_OBJECT_CLASS (demo_window_parent_class)->finalize (object);
}

static void
demo_window_render (DemoWindow *window)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (window->text_view));

    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter (buffer, &start_iter);
    gtk_text_buffer_get_end_iter (buffer, &end_iter);

    char *text = gtk_text_buffer_get_text (buffer, &start_iter, &end_iter, false);

    if (window->view)
    {
        g_clear_pointer (&window->view, g_object_unref);
        g_clear_pointer (&window->document, g_object_unref);
        g_clear_pointer (&window->error, g_object_unref);
    }

    GError *error = NULL;
    window->document = lsm_dom_document_new_from_memory (text, -1, &error);

    if (error != NULL)
    {
        GtkWidget *dlg = gtk_message_dialog_new (GTK_WINDOW (window),
                                                 GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Could not read MathML: %s\n",
                                                 error->message);

        gtk_window_present (GTK_WINDOW (dlg));
        g_signal_connect (dlg, "response", G_CALLBACK (gtk_window_destroy), NULL);

        g_error_free (error);
    }
    else
    {
        window->view = lsm_dom_document_create_view (window->document);

        LsmMathmlMathElement *root = lsm_mathml_document_get_root_element (LSM_MATHML_DOCUMENT (window->document));
        g_object_set (window->cursor,
                      "current", NULL,
                      NULL);
    }

    gtk_widget_queue_draw (GTK_WIDGET (window->lasem_view));
}

static void
draw_element_bbox (cairo_t          *cr,
                   LsmMathmlElement *element,
                   double            scale,
                   double            baseline,
                   gboolean          two_tone)
{
    cairo_save (cr);

    cairo_scale (cr, scale, scale);
    cairo_translate (cr, 0, baseline);
    cairo_set_source_rgba (cr, 246/255.0, 97/255.0, 81/255.0, 0.2);
    cairo_rectangle (cr,
                     element->x,
                     element->y,
                     element->bbox.width,
                     -element->bbox.height);
    cairo_fill (cr);

    if (two_tone)
        cairo_set_source_rgba (cr, 153/255.0, 193/255.0, 241/255.0, 0.2);
    cairo_rectangle (cr,
                     element->x,
                     element->y,
                     element->bbox.width,
                     element->bbox.depth);
    cairo_fill (cr);

    cairo_restore (cr);
}

static void
lasem_view_draw (GtkDrawingArea *drawing_area,
                 cairo_t        *cr,
                 int             width,
                 int             height,
                 DemoWindow     *window)
{
    if (window->view == NULL)
        return;

    window->multiplier = 3;
    lsm_dom_view_set_resolution (window->view, window->multiplier * LSM_DOM_VIEW_DEFAULT_RESOLUTION);

    lsm_dom_view_render (window->view, cr, 0, 0);

    double baseline = 0;
    lsm_dom_view_get_size (window->view, NULL, NULL, &baseline);

    /*if (window->pick)
        draw_element_bbox (cr, window->pick, window->multiplier, baseline, TRUE);*/

    /*LsmMathmlBbox selection_bbox;
    LsmMathmlElement *common = lsm_mathml_selection_get_selection_bounds (window->selection, &selection_bbox);
    if (common)
        draw_element_bbox (cr, common, window->multiplier, baseline, FALSE);*/

    LsmMathmlPosition *pos;
    g_object_get (window->cursor, "current", &pos, NULL);

    if (LSM_IS_MATHML_POSITION (pos))
    {
        int position;
        LsmMathmlElement *parent;
        g_object_get (pos, "parent", &parent, "position", &position, NULL);

        cairo_save (cr);

        double x_pos;
        double y_pos;

        if (position == 0)
            x_pos = parent->x;
        else
            x_pos = parent->x + parent->bbox.width;

        y_pos = parent->y;

        cairo_scale (cr, window->multiplier, window->multiplier);
        cairo_translate (cr, 0, baseline);
        cairo_set_source_rgb (cr, 1, 0, 0);
        cairo_rectangle (cr, x_pos, y_pos, 0.5,
                         -parent->bbox.height);
        cairo_rectangle (cr, x_pos, y_pos, 0.5,
                         parent->bbox.depth);
        cairo_fill (cr);

        cairo_restore (cr);
    }
}

gboolean
cb_key_pressed (GtkEventControllerKey *controller,
                guint                  keyval,
                guint                  keycode,
                GdkModifierType        state,
                DemoWindow            *self)
{
    gboolean handled = FALSE;

    LsmMathmlMathElement *root = lsm_mathml_document_get_root_element (self->document);

    if (keyval == GDK_KEY_Left) {
        lsm_mathml_cursor_move (self->cursor, root, LSM_DIRECTION_LEFT);
        handled = TRUE;
    }
    else if (keyval == GDK_KEY_Right) {
        lsm_mathml_cursor_move (self->cursor, root, LSM_DIRECTION_RIGHT);
        handled = TRUE;
    }
    else if (keyval == GDK_KEY_space) {
        lsm_mathml_cursor_get_insertion_points (root);
        handled = TRUE;
    }
    else if (keyval == GDK_KEY_a) {
        g_print ("a");
        LsmDomNode *ident = lsm_mathml_identifier_element_new ();
        lsm_dom_node_append_child (ident, LSM_DOM_NODE (lsm_dom_text_new ("a")));
        lsm_mathml_cursor_insert (self->cursor, LSM_MATHML_ELEMENT (ident));
        gtk_widget_queue_draw (GTK_WIDGET (self->lasem_view));
    }
    else if (keyval == GDK_KEY_Delete) {
        lsm_mathml_cursor_delete (self->cursor, FALSE);
    }
    else if (keyval == GDK_KEY_BackSpace) {
        lsm_mathml_cursor_delete (self->cursor, TRUE);
    }
    /*else {
        guint32 *unichar = gdk_keyval_to_unicode (keyval);
        g_print ("%d\n", unichar);
    }*/

    gtk_widget_queue_draw (GTK_WIDGET (self->lasem_view));
    return handled;
}

static void
cb_motion (GtkEventControllerMotion *controller,
           gdouble                   x,
           gdouble                   y,
           DemoWindow               *self)
{
    if (self->view == NULL)
        return;

    self->pick = lsm_mathml_view_pick (self->view,
                                       x / self->multiplier,
                                       y / self->multiplier);

    if (self->pick != NULL)
    {
        const char *fmt;
        fmt = g_strdup_printf ("Picked '%s' (<%s>) at:\nX: %.2lf\nY: %.2lf\nElement X: %.2lf\nElement Y: %.2lf\nElement Width: %.2lf\nElement Height: %.2lf\nElement Depth: %.2lf\n",
                               g_type_name_from_instance ((GTypeInstance *)self->pick),
                               lsm_dom_node_get_node_name (LSM_DOM_NODE (self->pick)),
                               x, y, // TODO: These are GTK positions, not equation coords
                               self->pick->x,
                               self->pick->y,
                               self->pick->bbox.width,
                               self->pick->bbox.height,
                               self->pick->bbox.depth);

        gtk_label_set_label (self->status_label, fmt);
    }
    else
        gtk_label_set_label (self->status_label, NULL);

    GtkWidget *lasem_view = gtk_event_controller_get_widget (GTK_EVENT_CONTROLLER (controller));
    gtk_widget_queue_draw (lasem_view);

    LsmMathmlElement *root = lsm_mathml_document_get_root_element (self->document);
    double baseline = 0;
    lsm_dom_view_get_size (self->view, NULL, NULL, &baseline); // DON'T DO THIS HERE !!

    LsmMathmlPosition *pos;
    if (self->pick != NULL)
        pos = lsm_mathml_cursor_get_nearest_insertion_point (self->pick, x / self->multiplier, (y / self->multiplier) - baseline);
    else
        pos = lsm_mathml_cursor_get_nearest_insertion_point (root, x / self->multiplier, (y / self->multiplier) - baseline);
    g_object_set (self->cursor, "current", pos, NULL);
}

static void
cb_drag_begin (GtkGestureDrag *gesture,
               gdouble         x,
               gdouble         y,
               DemoWindow     *self)
{
    LsmMathmlElement *root = lsm_mathml_document_get_root_element (self->document);
    double baseline = 0;
    lsm_dom_view_get_size (self->view, NULL, NULL, &baseline); // DON'T DO THIS HERE !!

    LsmMathmlPosition *start_pos;
    if (self->pick != NULL)
        start_pos = lsm_mathml_cursor_get_nearest_insertion_point (self->pick, x / self->multiplier, (y / self->multiplier) - baseline);
    else
        start_pos = lsm_mathml_cursor_get_nearest_insertion_point (root, x / self->multiplier, (y / self->multiplier) - baseline);

    g_object_set (self->selection, "start-position", start_pos, "end-position", start_pos, NULL);

    gtk_widget_queue_draw (GTK_WIDGET (self->lasem_view));
}

static void
cb_drag_update_or_end (GtkGestureDrag *gesture,
                       gdouble         x,
                       gdouble         y,
                       DemoWindow     *self)
{
    LsmMathmlElement *root = lsm_mathml_document_get_root_element (self->document);
    double baseline = 0;
    lsm_dom_view_get_size (self->view, NULL, NULL, &baseline); // DON'T DO THIS HERE !!

    LsmMathmlPosition *cur_pos;
    if (self->pick != NULL)
        cur_pos = lsm_mathml_cursor_get_nearest_insertion_point (self->pick, x / self->multiplier, (y / self->multiplier) - baseline);
    else
        cur_pos = lsm_mathml_cursor_get_nearest_insertion_point (root, x / self->multiplier, (y / self->multiplier) - baseline);

    g_object_set (self->selection, "end-position", cur_pos, NULL);

    gtk_widget_queue_draw (GTK_WIDGET (self->lasem_view));
}

static void
cb_pressed (GtkGestureClick *gesture,
            gint n_press,
            gdouble x,
            gdouble y,
            DemoWindow *self)
{
    GtkWidget *lasem_view = GTK_WIDGET (self->lasem_view);
    gtk_widget_grab_focus (lasem_view);
}

static void
demo_window_class_init (DemoWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = demo_window_finalize;

    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource (widget_class, "/com/mattjakeman/lsm-demo/demo.ui");

    gtk_widget_class_bind_template_child (widget_class, DemoWindow, lasem_view);
    gtk_widget_class_bind_template_child (widget_class, DemoWindow, text_view);
    gtk_widget_class_bind_template_child (widget_class, DemoWindow, status_label);
    gtk_widget_class_bind_template_child (widget_class, DemoWindow, render_btn);
}

static void
demo_window_init (DemoWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));

    self->cursor = lsm_mathml_cursor_new ();
    self->selection = lsm_mathml_selection_new ();

    // Render Button
    gtk_widget_add_css_class (GTK_WIDGET (self->render_btn), "suggested-action");
    g_signal_connect_swapped (self->render_btn, "clicked", G_CALLBACK (demo_window_render), self);

    // Set text view placeholder contents
    gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (self->text_view)), mml, -1);

    // Drawing area for Lasem Equation
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (self->lasem_view),
                                    (GtkDrawingAreaDrawFunc) lasem_view_draw,
                                    self, NULL);

    // Respond to key input (for cursor)
    GtkEventController *key_controller = gtk_event_controller_key_new ();
    g_signal_connect (key_controller, "key-pressed", G_CALLBACK (cb_key_pressed), self);
    gtk_widget_add_controller (GTK_WIDGET (self->lasem_view), key_controller);

    // Respond to mouse movement (for picking)
    GtkEventController *motion_controller = gtk_event_controller_motion_new ();
    g_signal_connect (motion_controller, "motion", G_CALLBACK (cb_motion), self);
    gtk_widget_add_controller (GTK_WIDGET (self->lasem_view), motion_controller);

    // Respond to mouse click (for focus)
    GtkGesture *click_gesture = gtk_gesture_click_new ();
    g_signal_connect (click_gesture, "pressed", G_CALLBACK (cb_pressed), self);
    gtk_widget_add_controller (GTK_WIDGET (self->lasem_view), GTK_EVENT_CONTROLLER (click_gesture));

    // Respond to mouse drag (for selections)
    GtkGesture *drag_gesture = gtk_gesture_drag_new ();
    g_signal_connect (drag_gesture, "drag-begin", G_CALLBACK (cb_drag_begin), self);
    g_signal_connect (drag_gesture, "drag-update", G_CALLBACK (cb_drag_update_or_end), self);
    g_signal_connect (drag_gesture, "drag-end", G_CALLBACK (cb_drag_update_or_end), self);
    gtk_widget_add_controller (GTK_WIDGET (self->lasem_view), GTK_EVENT_CONTROLLER (drag_gesture));

    demo_window_render (self);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    // Create and show a new DemoWindow instance
    GtkWidget *window = demo_window_new(app);
    gtk_widget_show (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("com.mattjakeman.LasemDemo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
