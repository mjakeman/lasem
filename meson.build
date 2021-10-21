project('lasem', 'c', version : '0.7.0')

as_version = meson.project_version()
ver_arr = as_version.split('.')

as_major_version = ver_arr[0]
as_minor_version = ver_arr[1]
as_micro_version = ver_arr[2]

lasem_api_version = as_major_version + '.' + as_minor_version

# setup directories
datadir = get_option('datadir')
libdir = get_option('libdir')
girdir = get_option('datadir') / 'gir-1.0'
typelibdir = libdir / 'girepository-1.0'
pkg_locale_dir = datadir / 'lasem' / 'locale'

gettext_package = 'lasem-' + lasem_api_version

# config.h file
conf = configuration_data()
conf.set_quoted('VERSION', as_version)
conf.set_quoted('GETTEXT_PACKAGE', gettext_package)
conf.set_quoted('PACKAGE_LOCALE_DIR', pkg_locale_dir)

configure_file(output : 'config.h', configuration : conf)

add_project_arguments('-DG_LOG_DOMAIN="Lasem"', language : 'c')

# TODO: Features
# TODO: Compiler Warnings

# build itex2mml
subdir('itex2mml')

# dependencies
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

# subdirectories
subdir('src')
subdir('po')
subdir('tests')
subdir('docs')

summary(
	{
		'API Docs': get_option('docs').enabled(),
		'Introspection': get_option('introspection').enabled(),
		'Tests': get_option('tests').enabled()
	},
	section: 'Options'
)
