#!/bin/sh

echo "running aclocal..."
aclocal

echo "running autoconf..."
autoconf

echo "running autoheader..."
autoheader

echo "running libtool..."
libtoolize --automake --copy --force

echo "running automake..."
automake --add-missing -c

echo "running configure..."
./configure

echo "'make' to build whole server."

