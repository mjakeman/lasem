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
pkg_locale_dir = join_paths(datadir, 'lasem', 'locale')

gettext_package = 'lasem-' + lasem_api_version
add_project_arguments('-DGETTEXT_PACKAGE="' + gettext_package + '"', language: 'c')

add_project_arguments('-DG_LOG_DOMAIN="Lasem"', language : 'c')
add_project_arguments('-DPACKAGE_LOCALE_DIR="' + pkg_locale_dir + '"', language : 'c')
add_project_arguments('-DPACKAGE_DATA_DIR="' + datadir + '"', language : 'c')

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
