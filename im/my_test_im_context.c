/*
 * Copiright (C) 2018 Santiago León O.
 */
#include <gtk/gtk.h>
#include "../util.c"
#include "my_test_im_context.h"

struct _MyTestIMContext {
    GtkIMContext parent_instance;
    gchar buff[10];
};

static void my_test_im_context_finalize (GObject *obj);
static gboolean my_test_im_context_filter_keypress (GtkIMContext *context, GdkEventKey *event);

G_DEFINE_DYNAMIC_TYPE (MyTestIMContext, my_test_im_context, GTK_TYPE_IM_CONTEXT);

void
my_test_im_context_register_type_external (GTypeModule *type_module)
{
    my_test_im_context_register_type (type_module);
}

static void
my_test_im_context_class_init (MyTestIMContextClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(class);
    gobject_class->finalize = my_test_im_context_finalize;

    GtkIMContextClass *gtk_im_context_class = GTK_IM_CONTEXT_CLASS(class);
    gtk_im_context_class->filter_keypress = my_test_im_context_filter_keypress;
}

MyTestIMContext*
my_test_im_context_new (void)
{
    return MY_TEST_IM_CONTEXT(g_object_new (MY_TEST_TYPE_IM_CONTEXT, NULL));
}

static void
my_test_im_context_class_finalize (MyTestIMContextClass *class)
{
    //printf ("Call to: my_test_im_context_class_finalize\n");
}

static void
my_test_im_context_init (MyTestIMContext *context)
{
    //printf ("Call to: my_test_im_context_init\n");
}

static void
my_test_im_context_finalize (GObject *context)
{
    //printf ("Call to: my_test_im_context_finalize\n");
}

static gboolean
my_test_im_context_filter_keypress_gdk_simple (GtkIMContext *context, GdkEventKey *event)
{
    gunichar uni = gdk_keyval_to_unicode (event->keyval);

    if (!g_unichar_iscntrl (uni)) {
        gchar buff[10];
        int len = g_unichar_to_utf8 (uni, buff);
        buff[len] = '\0';

        if (*buff != '\0' && event->type == GDK_KEY_PRESS ) {
            g_signal_emit_by_name (context, "commit", &buff);
            return TRUE;
        }
    }

    return FALSE;
}

static gboolean
my_test_im_context_filter_keypress_gdk_translate (GtkIMContext *context, GdkEventKey *event)
{
    //printf ("Call to: my_test_im_context_filter_keypress\n");
    gdk_event_key_print (event);
    printf ("\n");

    GdkDisplay *disp = gdk_display_get_default ();
    GdkKeymap *keymap = gdk_keymap_get_for_display (disp);

    guint keyval;
    GdkModifierType consumed;
    gboolean translated = gdk_keymap_translate_keyboard_state (keymap,
                                                               event->hardware_keycode,
                                                               event->state,
                                                               event->group,
                                                               &keyval,
                                                               NULL,
                                                               NULL,
                                                               &consumed);
    gunichar uni = gdk_keyval_to_unicode (keyval);
    gchar *str = MY_TEST_IM_CONTEXT(context)->buff;
    int len = g_unichar_to_utf8 (uni, str);
    str[len] = '\0';
    if (*str != '\0') {
        printf ("Translated: %s\n", translated ? "TRUE" : "FALSE");
        printf ("Translated str: ");
        print_str_hex (str);
        printf ("Consumed: ");
        gdk_modifier_type_print (consumed);
        printf ("\n\n");
        g_signal_emit_by_name (context, "commit", str);
        return TRUE;
    }

    printf ("\n");

    event->state &= ~consumed;

    return FALSE;
}

static gboolean
my_test_im_context_filter_keypress (GtkIMContext *context, GdkEventKey *event)
{
    //printf ("Call to: my_test_im_context_filter_keypress\n");

    switch (0) {
        case 0:
            return my_test_im_context_filter_keypress_gdk_simple (context, event);
        case 1:
            return my_test_im_context_filter_keypress_gdk_translate (context, event);
        default:
            return FALSE;
    }
}
