#include <gtk/gtk.h>
#include <xkbcommon/xkbcommon.h>

#include "common.h"
#include "bit_operations.c"
#include "util.c"

int num_mods;
const char **mod_names;
struct xkb_state *state;

#define GDK_MODIFIERS_TABLE                              \
  GDK_MODIFIERS_ROW(GDK_SHIFT_MASK)                      \
  GDK_MODIFIERS_ROW(GDK_LOCK_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_CONTROL_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_MOD1_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_MOD2_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_MOD3_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_MOD4_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_MOD5_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_BUTTON1_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_BUTTON2_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_BUTTON3_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_BUTTON4_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_BUTTON5_MASK)                    \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_13_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_14_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_15_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_16_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_17_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_18_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_19_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_20_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_21_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_22_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_23_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_24_MASK)       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_25_MASK)       \
  GDK_MODIFIERS_ROW(GDK_SUPER_MASK)                      \
  GDK_MODIFIERS_ROW(GDK_HYPER_MASK)                      \
  GDK_MODIFIERS_ROW(GDK_META_MASK)                       \
  GDK_MODIFIERS_ROW(GDK_MODIFIER_RESERVED_29_MASK)       \
  GDK_MODIFIERS_ROW(GDK_RELEASE_MASK)
  
char *gdk_modifier_names[32] = {0};

void gdk_modifier_names_init ()
{
#define GDK_MODIFIERS_ROW(name) \
    gdk_modifier_names[bit_pos(name)] = #name;
    GDK_MODIFIERS_TABLE
#undef GDK_MODIFIERS_ROW
}

static gint key_press (GtkWidget *widget, GdkEventKey *event)
{
    xkb_keycode_t keycode = event->hardware_keycode;
    enum xkb_state_component changed;
    printf ("type: ");
    if (event->type == GDK_KEY_PRESS) {
        printf ("KEY_PRESS\n");
        changed = xkb_state_update_key(state, keycode, XKB_KEY_DOWN);
    } else if (event->type == GDK_KEY_RELEASE) {
        printf ("KEY_RELEASE\n");
        changed = xkb_state_update_key(state, keycode, XKB_KEY_UP);
    } else {
        invalid_code_path;
    }

    printf ("state: ");
    bool is_first = true;
    for (uint32_t mask = event->state;
         mask;
         mask = mask & (mask-1)) {
        uint32_t next_bit_mask = mask & -mask;

        if (!is_first) {
            printf (", ");
        }
        is_first = false;
        printf ("%s", gdk_modifier_names[bit_pos(next_bit_mask)]);
    }
    printf ("\n");

    printf ("Changed: %x\n", changed);

    xkb_keysym_t keysym = xkb_state_key_get_one_sym(state, event->hardware_keycode);

    char keysym_name[64];
    xkb_keysym_get_name(keysym, keysym_name, sizeof(keysym_name));

    printf ("keysym: %s\n", keysym_name);

    int size = xkb_state_key_get_utf8(state, keycode, NULL, 0) + 1;
    if (size > 1) {
        char *buffer = malloc (size);
        xkb_state_key_get_utf8(state, keycode, buffer, size);
        printf ("UTF-8: %s\n", buffer);
    }

    printf ("Effective Mods: ");
    for (int i=0; i<num_mods; i++) {
        if (xkb_state_mod_index_is_active(state, i, XKB_STATE_MODS_EFFECTIVE)) {
            printf ("%s ", mod_names[i]);
        }
    }
    printf ("\n");

    printf ("Consumed Mods (XKB): ");
    for (int i=0; i<num_mods; i++) {
        if (xkb_state_mod_index_is_consumed2 (state, keycode, i, XKB_CONSUMED_MODE_XKB)) {
            printf ("%s ", mod_names[i]);
        }
    }
    printf ("\n");

    printf ("Consumed Mods (GTK): ");
    for (int i=0; i<num_mods; i++) {
        if (xkb_state_mod_index_is_consumed2 (state, keycode, i, XKB_CONSUMED_MODE_GTK)) {
            printf ("%s ", mod_names[i]);
        }
    }
    printf ("\n");

    printf ("\n");

    return FALSE;
}

struct cli_opt_t {
    int id;
    const char *opt;
    bool expect_arg;
    const char *arg;
    void *data;

    struct cli_opt_t *next;
};

struct cli_parser_t {
    mem_pool_t pool;
    int num_opts;
    struct cli_opt_t *opts;
    struct cli_opt_t *last_opt;

    int argv_idx;

    int error;
    char *error_msg;
};

int cli_parser_add_opt (struct cli_parser_t *parser, const char *opt, bool expect_arg, void *data)
{
    assert (opt && "non option arguments not yet implemented.");

    struct cli_opt_t *new_opt = mem_pool_push_size (&parser->pool, sizeof(struct cli_opt_t));
    *new_opt = ZERO_INIT(struct cli_opt_t);

    new_opt->opt = opt;
    new_opt->expect_arg = expect_arg;
    new_opt->data = data;
    if (parser->opts == NULL) {
        parser->opts = new_opt;
    } else {
        parser->last_opt->next = new_opt;
    }
    parser->last_opt = new_opt;

    return parser->num_opts++;
}

bool cli_parser_opt_lookup (struct cli_parser_t *parser, const char *opt_name, struct cli_opt_t *opt)
{
    struct cli_opt_t *curr_opt = parser->opts;
    while (curr_opt != NULL) {
        if (strcmp (opt_name, curr_opt->opt) == 0) {
            if (opt != NULL) {
                *opt = *curr_opt;
            }
            return true;
        }

        curr_opt = curr_opt->next;
    }
    return false;
}

enum cli_parser_err_t {
    CP_ERR_NO_ERROR,
    CP_ERR_MISSING_ARGUMENT,
    CP_ERR_UNRECOGNIZED_OPT
};

bool cli_parser_get_next (struct cli_parser_t *parser, int argc, char *argv[], struct cli_opt_t *opt)
{
    bool retval = false;
    assert (opt != NULL);

    if (parser->argv_idx + 1 < argc) {
        int idx = parser->argv_idx + 1;
        if (cli_parser_opt_lookup (parser, argv[idx], opt)) {
            if (opt->expect_arg) {
                if (idx + 1 < argc && !cli_parser_opt_lookup (parser, argv[idx + 1], NULL)) {
                    opt->arg = argv[++idx];
                    retval = true;

                } else {
                    // Missing argument
                    parser->error = CP_ERR_MISSING_ARGUMENT;
                    parser->error_msg = pprintf (&parser->pool, "Missing argument for option '%s'", opt->opt);
                }

            } else {
                // Successfully found a non argument option.
                retval = true;
            }

        } else {
            // Option not found
            parser->error = CP_ERR_UNRECOGNIZED_OPT;
            parser->error_msg = pprintf (&parser->pool, "Unrecognized option '%s'", argv[idx]);
        }

        if (retval == true) {
            parser->argv_idx += idx;
        }
    }

    return retval;
}

void cli_parser_destroy (struct cli_parser_t *parser)
{
    mem_pool_destroy (&parser->pool);
}

int main(int argc, char *argv[])
{
    gdk_modifier_names_init ();

    char *rules = NULL;
    char *model = NULL;
    char *layout = NULL;
    char *variant = NULL;
    char *options = NULL;
    bool got_keymap_names = true;

    // Compute the keymap names of the layout that will be tested.
    {
        if (argc == 1) {
            // TODO: Get current keymap names
        } else {
            struct cli_parser_t cli_parser = {0};
            cli_parser_add_opt (&cli_parser, "-r", true, &rules);
            cli_parser_add_opt (&cli_parser, "-m", true, &model);
            cli_parser_add_opt (&cli_parser, "-l", true, &layout);
            cli_parser_add_opt (&cli_parser, "-v", true, &variant);
            cli_parser_add_opt (&cli_parser, "-o", true, &options);

            struct cli_opt_t opt;
            while (cli_parser_get_next (&cli_parser, argc, argv, &opt) &&
                   cli_parser.error == CP_ERR_NO_ERROR)
            {
                *(const char **)opt.data = opt.arg;
            }

            if (cli_parser.error) {
                printf ("Error: %s\n", cli_parser.error_msg);
                got_keymap_names = false;
            }

            cli_parser_destroy (&cli_parser);

        }

        if (got_keymap_names == false) {
            return 0;
        } else {
            printf ("Used xkb_rule_names:\n");
            printf ("  rules: %s\n", rules);
            printf ("  model: %s\n", model);
            printf ("  layout: %s\n", layout);
            printf ("  variant: %s\n", variant);
            printf ("  options: %s\n", options);
            printf ("\n");
        }
    }

    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "GTK window");

    g_signal_connect (window, "key-press-event",
                      G_CALLBACK (key_press), NULL);
    g_signal_connect (window, "key-release-event",
                      G_CALLBACK (key_press), NULL);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    struct xkb_context *ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!ctx) {
        printf ("Could not create xkb context.\n");
    }

    struct xkb_rule_names names = {
        .rules = rules,
        .model = model,
        .layout = layout,
        .variant = variant,
        .options = options
    };
    struct xkb_keymap *keymap = xkb_keymap_new_from_names(ctx, &names,
                                                          XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap) {
        printf ("Could not create keymap.\n");
    }

    num_mods = xkb_keymap_num_mods (keymap);
    mod_names = malloc (sizeof(char *)*num_mods);
    printf ("Num Mods: %d\n", num_mods);
    for (int i=0; i<num_mods; i++) {
        mod_names[i] = xkb_keymap_mod_get_name(keymap, i);
        printf ("%s ", mod_names[i]);
    }
    printf ("\n\n");

    state = xkb_state_new(keymap);
    if (!state) {
        printf ("Could not create xkb state.\n");
    }

    gtk_main();

    return 0;
}

