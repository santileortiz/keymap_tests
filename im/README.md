Input method testing
====================

Simple input method implementation to test how applications use them.

**NOTE:** Commands should be executed from the repository's root.

### Dependencies
Install build dependencies in elementaryOS/Ubuntu with:
```
$ sudo apt-get install libgtk-3-dev libgtk2.0-dev
```

### Compilation
To build for GTK3 use:
```
$ ./pymk im_gtk3
```

or for GTK2:
```
$ ./pymk im_gtk2
```

### Testing
A GTK2 or GTK3 test application will be built into ./bin/, to test the IM with it use:
```
$ GTK_IM_MODULE=my_test_im ./bin/test_im 
```

