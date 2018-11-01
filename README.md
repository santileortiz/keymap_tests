Tests related to keymap handling
================================

Keymap Upload Performance
-------------------------

This test compares the performance of 2 ways of uploading a keymap to X11.

Install the dependencies and build with:
```
$ sudo apt-get libgtk-3-dev
$ ./pymk keymap_upload
```

Then run the test with:
```
$ ./bin/keymap_upload
```

A text entry should appear, while focused on it, pressing TAB should switch to
the 'br' layout, then pressing TAB again will switch to the 'us' layout. Try
for example pressing the ; key several times then press tab, it should start to
print ç.

The way Clutter does this (Gala uses Mutter, which in turn uses Clutter) can be
toggled by changing the 0 to 1 in the function `key_press()` (the line with
`#if 0`), and then recompiling with:
```
$ ./pymk keymap_upload
```
