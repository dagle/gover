#include "gover-canvas.h"

#include <gtk4-layer-shell.h>
/**
 * GoverCanvas:
 *
 * A canvas widget to draw on
 *
 */

struct _GoverCanvas {
  GtkWindow parent_instance;

  int left_margin;
  int right_margin;
  int top_margin;
  int bottom_margin;
  uint screen_num;

  int height;
};

G_DEFINE_TYPE(GoverCanvas, g_gover_canvas, GTK_TYPE_WINDOW)

enum {
  PROP_0,

  PROP_LEFT_MARGIN,
  PROP_RIGHT_MARGIN,
  PROP_TOP_MARGIN,
  PROP_BOTTOM_MARGIN,

  PROP_SCREEN_NUM,

  NUM_PROPERTIES,
};

static GParamSpec *canvas_props[NUM_PROPERTIES] = {
    NULL,
};

static void g_gover_cavnas_set_property(GObject *object, guint property_id,
                                        const GValue *value,
                                        GParamSpec *pspec) {
  GoverCanvas *canvas = GOVER_CANVAS(object);

  switch (property_id) {
  case PROP_LEFT_MARGIN:
    canvas->left_margin = g_value_get_uint(value);
    break;
  case PROP_RIGHT_MARGIN:
    canvas->right_margin = g_value_get_uint(value);
    break;
  case PROP_TOP_MARGIN:
    canvas->top_margin = g_value_get_uint(value);
    break;
  case PROP_BOTTOM_MARGIN:
    canvas->bottom_margin = g_value_get_uint(value);
    break;
  case PROP_SCREEN_NUM:
    canvas->screen_num = g_value_get_uint(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void g_gover_canvas_get_property(GObject *object, guint property_id,
                                        GValue *value, GParamSpec *pspec) {

  GoverCanvas *canvas = GOVER_CANVAS(object);

  switch (property_id) {
  case PROP_LEFT_MARGIN:
    g_value_set_uint(value, canvas->left_margin);
    break;
  case PROP_RIGHT_MARGIN:
    g_value_set_uint(value, canvas->right_margin);
    break;
  case PROP_TOP_MARGIN:
    g_value_set_uint(value, canvas->top_margin);
    break;
  case PROP_BOTTOM_MARGIN:
    g_value_set_uint(value, canvas->bottom_margin);
    break;
  case PROP_SCREEN_NUM:
    g_value_set_uint(value, canvas->screen_num);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
  }
}

static void g_gover_canvas_constructed(GObject *object);

static void g_gover_canvas_class_init(GoverCanvasClass *class) {
  GObjectClass *gobject_class = G_OBJECT_CLASS(class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

  gobject_class->set_property = g_gover_cavnas_set_property;
  gobject_class->get_property = g_gover_canvas_get_property;
  gobject_class->constructed = g_gover_canvas_constructed;

  /**
   * BarBarBackground:left-margin:
   *
   * Padding from the right
   */
  canvas_props[PROP_LEFT_MARGIN] =
      g_param_spec_uint("left-margin", NULL, NULL, 0, G_MAXUINT, 0,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
  /**
   * BarBarBackground:right-margin:
   *
   * Padding from the right
   */
  canvas_props[PROP_RIGHT_MARGIN] =
      g_param_spec_uint("right-margin", NULL, NULL, 0, G_MAXUINT, 0,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
  /**
   * BarBarBackground:top-margin:
   *
   * Padding from the top
   */
  canvas_props[PROP_TOP_MARGIN] =
      g_param_spec_uint("top-margin", NULL, NULL, 0, G_MAXUINT, 0,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
  /**
   * BarBarBackground:bottom-margin:
   *
   * Padding from the bottom
   */
  canvas_props[PROP_BOTTOM_MARGIN] =
      g_param_spec_uint("bottom-margin", NULL, NULL, 0, G_MAXUINT, 0,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
  /**
   * BarBarBackground:screen-num:
   *
   * What screen to display the background on.
   */
  canvas_props[PROP_SCREEN_NUM] =
      g_param_spec_uint("screen-num", NULL, NULL, 0, G_MAXUINT, 0,
                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_properties(gobject_class, NUM_PROPERTIES,
                                    canvas_props);

  gtk_widget_class_set_css_name(widget_class, "background");
}

static void g_gover_canvas_init(GoverCanvas *self) {
  self->screen_num = 0;

  GtkWindow *gtk_window = GTK_WINDOW(self);

  gtk_layer_init_for_window(gtk_window);
}

static void g_gover_canvas_constructed(GObject *object) {
  G_OBJECT_CLASS(g_gover_canvas_parent_class)->constructed(object);
  GoverCanvas *background = GOVER_CANVAS(object);

  GtkWindow *gtk_window = GTK_WINDOW(object);

  gtk_layer_set_namespace(gtk_window, "canvas");
  // GTK_LAYER_SHELL_LAYER_TOP,
  // GTK_LAYER_SHELL_LAYER_OVERLAY,
  gtk_layer_set_layer(gtk_window, GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_auto_exclusive_zone_enable(gtk_window);

  // something like this, make it possible to identify the monitor by name
  GdkDisplay *gdk_display = gdk_display_get_default();
  GListModel *monitors = gdk_display_get_monitors(gdk_display);
  if (background->screen_num <= g_list_model_get_n_items(monitors)) {
    GdkMonitor *monitor =
        g_list_model_get_item(monitors, background->screen_num);
    if (monitor) {
      gtk_layer_set_monitor(gtk_window, monitor);
    }
  }

  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_LEFT,
                       background->left_margin);
  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_RIGHT,
                       background->right_margin);
  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_TOP,
                       background->top_margin);
  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_BOTTOM,
                       background->bottom_margin);

  for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
    gtk_layer_set_anchor(gtk_window, i, TRUE);
  }
}

/**
 * g_barbar_background_new:
 *
 * Creates a new `BarBarBackground`.
 *
 * Returns: (transfer full): a new `BarBarBackground`.
 */
GtkWidget *g_gover_canvas_new(void) {
  GoverCanvas *canvas;

  canvas = g_object_new(GOVER_TYPE_CANVAS, NULL);

  return GTK_WIDGET(canvas);
}
