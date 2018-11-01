#include <gtk/gtk.h>

#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include "common.h"
#include "util.c"
#include "slo_timers.h"

#define NUM_LAYOUTS 2
mem_pool_t pool = ZERO_INIT(mem_pool_t);
XkbComponentNamesRec *names[NUM_LAYOUTS];
XkbDescPtr precompiled[NUM_LAYOUTS];
int curr_layout = 0;

XkbComponentNamesRec* create_names (mem_pool_t *pool, char *layout)
{
    XkbComponentNamesRec *names = mem_pool_push_size (pool, sizeof (XkbComponentNamesRec));
    names->keymap = NULL;
    names->keycodes = "evdev+aliases(qwerty)";
    names->types = "complete";
    names->compat = "complete";
    names->symbols = pprintf(pool, "pc+%s+inet(evdev)", layout);
    names->geometry = "pc(pc105)";
    return names;
}

static gint key_press (GtkWidget *widget, GdkEventKey *event)
{
    if (event->keyval == GDK_KEY_Tab) {
        curr_layout = ((curr_layout + 1) % NUM_LAYOUTS);

        GdkDisplay *gdk_dpy = gdk_display_get_default ();
        Display *x11_dpy = gdk_x11_display_get_xdisplay (gdk_dpy);

#if 0
        BEGIN_WALL_CLOCK;
        XkbDescPtr desc_ptr = 
            XkbGetKeyboardByName(x11_dpy, XkbUseCoreKbd, names[curr_layout],
                                 XkbGBN_AllComponentsMask,
                                 XkbGBN_AllComponentsMask &
                                 (~XkbGBN_GeometryMask), True);
        PROBE_WALL_CLOCK("keymap compilation/upload");

        if (!desc_ptr) {
            printf ("Layout compilation/upload failed.\n");
        }
#else
        BEGIN_WALL_CLOCK;
        XkbSetMap (x11_dpy, XkbGBN_AllComponentsMask & (~XkbGBN_GeometryMask), precompiled[curr_layout]);
        PROBE_WALL_CLOCK("keymap upload");
#endif

        return TRUE;
    }

    return FALSE;
}

int main(int argc, char *argv[])
{
    setup_clocks ();

    GtkWidget *window;

    gtk_init(&argc, &argv);

    BEGIN_WALL_CLOCK;
    names[0] = create_names (&pool, "us");
    names[1] = create_names (&pool, "br");
    PROBE_WALL_CLOCK("names creation");

    GdkDisplay *gdk_dpy = gdk_display_get_default ();
    Display *x11_dpy = gdk_x11_display_get_xdisplay (gdk_dpy);
    BEGIN_WALL_CLOCK;
    precompiled[0] =
            XkbGetKeyboardByName(x11_dpy, XkbUseCoreKbd, names[0],
                                 XkbGBN_AllComponentsMask,
                                 XkbGBN_AllComponentsMask, False);
    precompiled[1] =
            XkbGetKeyboardByName(x11_dpy, XkbUseCoreKbd, names[1],
                                 XkbGBN_AllComponentsMask,
                                 XkbGBN_AllComponentsMask, False);
    PROBE_WALL_CLOCK("precompilation");

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "GTK window");

    g_signal_connect (window, "key-press-event",
                      G_CALLBACK (key_press), NULL);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *entry = gtk_entry_new ();
    gtk_container_add (GTK_CONTAINER(window), entry);

    gtk_widget_show_all(window);

    gtk_main();

    XFree (precompiled[0]);
    XFree (precompiled[1]);

    return 0;
}

