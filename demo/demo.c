#include <gtk/gtk.h>

#include <lsm.h>
#include <lsmdom.h>
#include <lsmmathml.h>

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
};

G_DEFINE_FINAL_TYPE (DemoWindow, demo_window, GTK_TYPE_APPLICATION_WINDOW)

/* see test.mml */
static const char *mml = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\r\n  <mrow>\r\n    <msub>\r\n      <mi>a</mi>\r\n      <mn>0</mn>\r\n    </msub>\r\n    <mo>+</mo>\r\n    <mfrac>\r\n      <mn>1</mn>\r\n      <mrow>\r\n        <msub>\r\n          <mi>a</mi>\r\n          <mn>1</mn>\r\n        </msub>\r\n        <mo>+</mo>\r\n        <mfrac>\r\n          <mn>1</mn>\r\n          <mrow>\r\n            <msub>\r\n              <mi>a</mi>\r\n              <mn>2</mn>\r\n            </msub>\r\n            <mo>+</mo>\r\n            <mfrac>\r\n              <mn>1</mn>\r\n\r\n              <mrow>\r\n                <msub>\r\n                  <mi>a</mi>\r\n                  <mn>3</mn>\r\n                </msub>\r\n                <mo>+</mo>\r\n                <mfrac>\r\n                  <mn>1</mn>\r\n                  <mrow>\r\n                    <msub>\r\n                      <mi>a</mi>\r\n                      <mn>4</mn>\r\n                    </msub>\r\n                  </mrow>\r\n                </mfrac>\r\n              </mrow>\r\n\r\n            </mfrac>\r\n          </mrow>\r\n        </mfrac>\r\n      </mrow>\r\n    </mfrac>\r\n  </mrow>\r\n</math>\r\n\0";

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
    }

    gtk_widget_queue_draw (GTK_WIDGET (window->lasem_view));
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

    if (window->pick)
    {
        double baseline = 0;
        lsm_dom_view_get_size (window->view, NULL, NULL, &baseline);

        cairo_scale (cr, window->multiplier, window->multiplier);
        cairo_translate (cr, 0, baseline);
        cairo_set_source_rgba (cr, 246/255.0, 97/255.0, 81/255.0, 0.2);
        cairo_rectangle (cr,
                         window->pick->x,
                         window->pick->y,
                         window->pick->bbox.width,
                         -window->pick->bbox.height);
        cairo_fill (cr);

        cairo_set_source_rgba (cr, 153/255.0, 193/255.0, 241/255.0, 0.2);
        cairo_rectangle (cr,
                         window->pick->x,
                         window->pick->y,
                         window->pick->bbox.width,
                         window->pick->bbox.depth);
        cairo_fill (cr);
    }
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

    // Render Button
    gtk_widget_add_css_class (GTK_WIDGET (self->render_btn), "suggested-action");
    g_signal_connect_swapped (self->render_btn, "clicked", G_CALLBACK (demo_window_render), self);

    // Set text view placeholder contents
    gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (self->text_view)), mml, -1);

    // Drawing area for Lasem Equation
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (self->lasem_view),
                                    (GtkDrawingAreaDrawFunc) lasem_view_draw,
                                    self, NULL);

    // Respond to mouse movement (for picking)
    GtkEventController *motion_controller = gtk_event_controller_motion_new ();
    g_signal_connect (motion_controller, "motion", G_CALLBACK (cb_motion), self);
    gtk_widget_add_controller (GTK_WIDGET (self->lasem_view), motion_controller);

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

    app = gtk_application_new ("com.mattjakeman.lsm-demo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
