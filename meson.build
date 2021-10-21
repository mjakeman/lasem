project('lasem', 'c', version : '0.7.0')

as_version = meson.project_version()
ver_arr = as_version.split('.')

as_major_version = ver_arr[0]
as_minor_version = ver_arr[1]
as_micro_version = ver_arr[2]

lasem_api_version = as_major_version + '.' + as_minor_version

conf = configuration_data()
# Surround the version in quotes to make it a C string
conf.set_quoted('VERSION', as_version)
configure_file(output : 'config.h',
               configuration : conf)

# TODO: Features
# TODO: Compiler Warnings

datadir = get_option('datadir')
girdir = get_option('datadir') / 'gir-1.0'
typelibdir = get_option('libdir') / 'girepository-1.0'
pkg_locale_dir = datadir / 'lasem' / 'locale'

gettext_package = 'lasem-' + lasem_api_version
add_project_arguments('-DGETTEXT_PACKAGE="' + gettext_package + '"', language: 'c')

add_project_arguments('-DG_LOG_DOMAIN="Lasem"', language : 'c')
add_project_arguments('-DPACKAGE_LOCALE_DIR="' + pkg_locale_dir + '"', language : 'c')

subdir('itex2mml')

deps = [
	dependency('glib-2.0'),
	dependency('gobject-2.0'),
	dependency('gio-2.0'),
	dependency('gdk-pixbuf-2.0', version : '>= 2.16'),
	dependency('cairo', version : '>= 1.2'),
	dependency('pangocairo', version : '>= 1.16.0'),
	dependency('libxml-2.0'),
	libitex2mml_dep
]

subdir('src')

# TODO: Configuration

# Yacc:                   bison -y
# Lex:                    flex
# Compiler:               gcc
# Compiler flags:         -g -O2 -Wformat=2 -Wmissing-format-attribute -Wextra -Wundef -Wstrict-prototypes -Wnested-externs -Wchar-subscripts -Wwrite-strings -Wmissing-noreturn -Wmissing-declarations -Wcast-align -Winline -Wno-pointer-sign -Wno-unused-parameter -Wno-sign-compare -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/sysprof-4 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/harfbuzz -I/usr/include/libxml2 -I/usr/include/pango-1.0 -I/usr/include/fribidi -pthread
