Title: Overview
Slug: overview

# Overview
Lasem is a GObject based library for the rendering and edition of Mathml and
SVG documents. It supports gobject-introspection, making it usable directly
from [Python (PyGObject)](https://pygobject.readthedocs.io/en/latest/),
[Javascript (GJS)](https://gjs.guide/), or [Vala](https://wiki.gnome.org/Projects/Vala).

## Building and debugging Lasem
On UNIX, Lasem uses the standard GNU build system, using
**autoconf** for package configuration and resolving portability issues,
**automake** for building makefiles that comply with the GNU Coding Standards,
and **libtool** for building shared libraries on multiple platforms. The normal
sequence for compiling and installing the Lasem library is thus:

```bash
./configure
make
make install
```

The standard options provided by **GNU autoconf** may be passed to the
`configure` script. Please see the **autoconf** documentation or run
`./configure --help` for information about the standard options.

The LSM_DEBUG environment variable can be set to a colon
separated list of debugging domains, which will make Lasem
print out different types of debugging informations to the
console. For example, the following command before running an
Lasem based application will make Lasem print out DOM
related informations:
		
`export LSM_DEBUG=dom`

## Required fonts
For the Mathml rendering, the following ttf fonts should be installed: cmr10, cmmi10, cmex10 and cmsy10.
They are provided by the lyx-fonts package in fedora, and the ttf-lyx package in debian/ubuntu.
