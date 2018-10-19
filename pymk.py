#!/usr/bin/python3
from mkpy.utility import *
import textwrap, re

assert sys.version_info >= (3,2)

modes = {
        'debug': '-O0 -g -Wall',
        'profile_debug': '-O2 -g -pg -Wall',
        'release': '-O2 -g -DNDEBUG -Wall'
        }
cli_mode = get_cli_option('-M,--mode', modes.keys())
FLAGS = modes[pers('mode', 'debug', cli_mode)]
GTK3_FLAGS = ex ('pkg-config --cflags --libs gtk+-3.0', ret_stdout=True, echo=False)
GTK2_FLAGS = ex ('pkg-config --cflags --libs gtk+-2.0', ret_stdout=True, echo=False)
ensure_dir ("bin")

def default():
    target = pers ('last_target', 'keyboard_layout_editor')
    call_user_function(target)

def libxkbcommon_test ():
    ex ('gcc {FLAGS} -o bin/libxkbcommon_test libxkbcommon_test.c {GTK3_FLAGS} -lm -lxkbcommon')

def im_gtk3 ():
    print ("BUILDING IM FOR GTK3")
    ex ('gcc -shared -fPIC {FLAGS} -o bin/my-test-im.so im/my_test_im_context.c im/my_test_im_module.c {GTK3_FLAGS} -lm -lxkbcommon')
    ex ('gcc {FLAGS} -o bin/test_im im/test_im.c {GTK3_FLAGS}')
    ex ('chmod 644 bin/my-test-im.so', echo=False)
    ex ('sudo cp bin/my-test-im.so /usr/lib/x86_64-linux-gnu/gtk-3.0/3.0.0/immodules')
    ex ('sudo /usr/lib/x86_64-linux-gnu/libgtk-3-0/gtk-query-immodules-3.0 --update-cache')

def im_gtk2 ():
    print ("BUILDING IM FOR GTK2")
    ex ('gcc -shared -fPIC {FLAGS} -o bin/my-test-im.so im/my_test_im_context.c im/my_test_im_module.c {GTK2_FLAGS} -lm -lxkbcommon')
    ex ('gcc {FLAGS} -o bin/test_im im/test_im.c {GTK2_FLAGS}')
    ex ('chmod 644 bin/my-test-im.so', echo=False)
    ex ('sudo cp bin/my-test-im.so /usr/lib/x86_64-linux-gnu/gtk-2.0/2.10.0/immodules')
    ex ('sudo /usr/lib/x86_64-linux-gnu/libgtk2.0-0/gtk-query-immodules-2.0 --update-cache')

if __name__ == "__main__":
    # Everything above this line will be executed for each TAB press.
    # If --get_completions is set, handle_tab_complete() calls exit().
    handle_tab_complete ()

    pymk_default()

