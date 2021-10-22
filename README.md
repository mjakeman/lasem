# Lasem
[![CI](https://github.com/mjakeman/lasem/actions/workflows/main.yml/badge.svg)](https://github.com/mjakeman/lasem/actions/workflows/main.yml)
[![Docs](https://img.shields.io/badge/documentation-visit-success)](https://www.mattjakeman.com/lasem/)

A SVG and MathML rendering library for mathematical equations.

## Status
Development on the upstream lasem library has been halted and the repository
is now read-only. This is an attempt at resurrecting lasem as a general purpose
library for rendering mathematical equations.

The current roadmap is:
 - [X] Port the build system to meson (in progress)
    - [X] Currently `lasem-render` can be built using meson
    - [X] Introspection, Locale, and Headers are installed correctly
 - [X] Deprecate and remove the old autotools build system
 - [X] Port the test suite
 - [X] Create a sample program demonstrating integration with GTK 4
 - [ ] Release "version 0.7"

## Documentation
The documentation for the `master` branch can be found
[here](https://www.mattjakeman.com/lasem/).

## Build
Lasem can be built using meson. Run the following commands:

```bash
# configure (see below for options)
meson _build

# build
meson compile -C _build

# install
meson install -C _build

# run tests (if enabled)
meson test -C _build -v
```

GObject Introspection data can be generated with `-Dintrospection=enabled`.

The API Reference can be generated with `-Ddocs=enabled`.

Tests can be built with `-Dtests=enabled`.

The demo viewer can be built with `-Ddemo=enabled`.

## Summary
Lasem aims to be a C/GObject based SVG/Mathml renderer and editor, supporting CSS
style sheets (only rendering is implemented for now). It uses cairo and pango as
it's rendering abstraction layer, and then support numerous output formats: xlib,
PNG, SVG, PDF, PS, EPS...

The API tries to follow the SVG and MathML DOM.

It currently includes two simple applications:

lasem-render, which takes a filename as argument and generates either a SVG, a PDF
or a PS file, depending on the output file extension. There's a debug mode option
(-d) that displays the bounding boxes of elements as blue rectangles, the logical
extents of text as red boxes and the ink extents of text as green boxes. It accepts
either MathML, Latex maths and SVG input. For the latex input, lasemrender uses
the embedded itex2mml library for the conversion to MathML.

lsm-test, which, if launched without argument, renders recursively all svg and mml
files in a directory, and generate an html file which displays the browser
rendering, lasem rendering, and if present the reference output. The html
filename is lasemtest.xml. It's main use is the rendering of the sample files
in the samples directory, by invoking "cd test && ./lsm-test data".

Requirements:

gobject, glib, gio, gdk-pixbuf, gdk, cairo, pangocairo, libxml, bison, flex

For the runtime, the following ttf fonts should be installed: cmr10, cmmi10, cmex10 and cmsy10.
They are provided by the lyx-fonts package in fedora, and the ttf-lyx package in debian/ubuntu.
