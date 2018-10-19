/*
 * Copiright (C) 2018 Santiago León O.
 */

void print_str_hex (const gchar *str)
{
    printf ("%s : ", str);
    while (*str) {
        printf ("%x ", (unsigned char)*str);
        str++;
    }
    printf ("\n");
}

void gdk_modifier_type_print (GdkModifierType mods)
{
    if (mods & GDK_SHIFT_MASK) {
        printf (" SHIFT_MASK");
    }

    if (mods & GDK_LOCK_MASK) {
        printf (" LOCK_MASK");
    }

    if (mods & GDK_CONTROL_MASK) {
        printf (" CONTROL_MASK");
    }

    if (mods & GDK_MOD1_MASK) {
        printf (" MOD1_MASK");
    }

    if (mods & GDK_MOD2_MASK) {
        printf (" MOD2_MASK");
    }

    if (mods & GDK_MOD3_MASK) {
        printf (" MOD3_MASK");
    }

    if (mods & GDK_MOD4_MASK) {
        printf (" MOD4_MASK");
    }

    if (mods & GDK_MOD5_MASK) {
        printf (" MOD5_MASK");
    }

    if (mods & GDK_BUTTON1_MASK) {
        printf (" BUTTON1_MASK");
    }

    if (mods & GDK_BUTTON2_MASK) {
        printf (" BUTTON2_MASK");
    }

    if (mods & GDK_BUTTON3_MASK) {
        printf (" BUTTON3_MASK");
    }

    if (mods & GDK_BUTTON4_MASK) {
        printf (" BUTTON4_MASK");
    }

    if (mods & GDK_BUTTON5_MASK) {
        printf (" BUTTON5_MASK");
    }

    if (mods & GDK_SUPER_MASK) {
        printf (" SUPER_MASK");
    }

    if (mods & GDK_HYPER_MASK) {
        printf (" HYPER_MASK");
    }

    if (mods & GDK_META_MASK) {
        printf (" META_MASK");
    }

    if (mods & ~GDK_MODIFIER_MASK) {
        printf (" (");
        if (mods & GDK_MODIFIER_RESERVED_13_MASK) {
            printf (" GDK_MODIFIER_RESERVED_13_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_14_MASK) {
            printf (" GDK_MODIFIER_RESERVED_14_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_15_MASK) {
            printf (" GDK_MODIFIER_RESERVED_15_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_16_MASK) {
            printf (" GDK_MODIFIER_RESERVED_16_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_17_MASK) {
            printf (" GDK_MODIFIER_RESERVED_17_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_18_MASK) {
            printf (" GDK_MODIFIER_RESERVED_18_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_19_MASK) {
            printf (" GDK_MODIFIER_RESERVED_19_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_20_MASK) {
            printf (" GDK_MODIFIER_RESERVED_20_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_21_MASK) {
            printf (" GDK_MODIFIER_RESERVED_21_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_22_MASK) {
            printf (" GDK_MODIFIER_RESERVED_22_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_23_MASK) {
            printf (" GDK_MODIFIER_RESERVED_23_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_24_MASK) {
            printf (" GDK_MODIFIER_RESERVED_24_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_25_MASK) {
            printf (" GDK_MODIFIER_RESERVED_25_MASK");
        }

        if (mods & GDK_MODIFIER_RESERVED_29_MASK) {
            printf (" GDK_MODIFIER_RESERVED_29_MASK");
        }

        if (mods & GDK_RELEASE_MASK) {
            printf (" GDK_RELEASE_MASK");
        }

        printf (" )");
    }
}

void gdk_keymap_print_intents ()
{
    GdkDisplay *disp = gdk_display_get_default ();
    GdkKeymap *keymap = gdk_keymap_get_for_display (disp);
    GdkModifierType mods;

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_PRIMARY_ACCELERATOR);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_PRIMARY_ACCELERATOR: ");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_CONTEXT_MENU);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_CONTEXT_MENU: ");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_EXTEND_SELECTION);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_EXTEND_SELECTION");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_MODIFY_SELECTION);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_MODIFY_SELECTION");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_NO_TEXT_INPUT);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_NO_TEXT_INPUT");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_SHIFT_GROUP);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_SHIFT_GROUP");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }

    mods = gdk_keymap_get_modifier_mask (keymap, GDK_MODIFIER_INTENT_DEFAULT_MOD_MASK);
    if (mods) {
        printf ("GDK_MODIFIER_INTENT_DEFAULT_MOD_MASK");
        gdk_modifier_type_print (mods);
        printf ("\n");
    }
}

void gdk_event_key_print (GdkEventKey *event)
{
    printf ("type: ");
    if (event->type == GDK_KEY_RELEASE) {
        printf ("GDK_KEY_RELEASE\n");
    } else if (event->type == GDK_KEY_PRESS) {
        printf ("GDK_KEY_PRESS\n");
    }

    printf ("window %p\n", event->window);
    printf ("send_event: %s\n", event->send_event ? "TRUE" : "FALSE");
    printf ("time: %u\n", event->time);

    printf ("state: ");
    gdk_modifier_type_print (event->state);
    printf ("\n");

    gunichar uni = gdk_keyval_to_unicode (event->keyval);
    gchar buff[10];
    int len = g_unichar_to_utf8 (uni, buff);
    buff[len] = '\0';
    printf ("keyval: %s\n", buff);

    printf ("length: %i\n", event->length);
    printf ("string: %.*s\n", event->length, event->string);
    printf ("hardware_keycode: %u\n", (unsigned int)event->hardware_keycode);
    printf ("group: %u\n", (unsigned int)event->group);
    printf ("is_modifier: %s\n", event->send_event ? "TRUE" : "FALSE");
}

