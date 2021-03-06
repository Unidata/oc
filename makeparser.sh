#!/bin/bash

# These rules are used if someone wants to rebuild
# daptab.c and/or daptab.h from dap.y.
# Otherwise never invoked, but records how to do it.
# BTW: note that renaming is essential because otherwise
# autoconf will forcibly delete files of the name *.tab.*

rm -f dap.tab.c dap.tab.h dapy.c dapy.h
bison --debug -d -p dap dap.y
sed -e s/dap.tab.c/dapy.c/g -e s/dap.tab.h/dapy.h/g <dap.tab.c >dapy.c
sed -e s/dap.tab.c/dapy.c/g -e s/dap.tab.h/dapy.h/g <dap.tab.h >dapy.h
