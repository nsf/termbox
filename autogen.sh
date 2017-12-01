#!/bin/bash

libtoolize --force --copy --automake
aclocal $ACLOCAL_FLAGS
autoconf
automake --gnu --add-missing --force --copy \
    -Wno-portability -Wno-portability

if [ ! -z "$NOCONFIGURE" ]; then
    echo "Done. ./configure skipped."
    exit $?
fi

{ cat <<EOF
#!/usr/bin/env bash
./autogen.sh $@ \$@
EOF
} > reautogen.sh
chmod +x reautogen.sh

exec ./configure $@
