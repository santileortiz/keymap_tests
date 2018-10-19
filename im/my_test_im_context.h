/*
 * Copiright (C) 2018 Santiago León O.
 */
#if !defined(MY_TEST_IM_CONTEXT_H)

G_BEGIN_DECLS

#define MY_TEST_TYPE_IM_CONTEXT (my_test_im_context_get_type())


// NOTE: glib-2.0 changed the definition of G_DECLARE_FINAL_TYPE to add chainup
// calls to glib_autoptr_cleanup_<ParentClass>(). GTK2 links against glib-2.0
// but GTK2's classes don't have this cleanup function, which causes implicit
// declarations here. To fix this we declare this function to do nothing.
//
// Glib added g_autoptr() support to G_DECLARE_FINAL_TYPE in commit 8ea414c8c6.
// Some GitFu tells me support for g_autoptr() was added after version 3.15.4.
// Hopefully this is correct, and support for GtkIMContext was added there.
#if !GTK_CHECK_VERSION(3,15,4)
static inline void glib_autoptr_cleanup_GtkIMContext (GtkIMContext **ptr) {}
#endif
G_DECLARE_FINAL_TYPE (MyTestIMContext, my_test_im_context, MY_TEST, IM_CONTEXT, GtkIMContext)

MyTestIMContext* my_test_im_context_new (void);
void my_test_im_context_register_type_external (GTypeModule *type_module);

G_END_DECLS

#define MY_TEST_IM_CONTEXT_H
#endif
