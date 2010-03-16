#!/bin/sh
aclocal
autoreconf -fi
automake --add-missing
intltoolize --force


