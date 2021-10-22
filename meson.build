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

cc = meson.get_compiler('c')

global_c_args = []
test_c_args = [
	'-Wformat=2',
	'-Wmissing-format-attribute',
	'-Wextra',
	'-Wundef',
	'-Wstrict-prototypes -Wnested-externs',
	'-Wchar-subscripts -Wwrite-strings',
	'-Wmissing-noreturn',
	'-Wmissing-declarations',
	'-Wcast-align',
	'-Winline',
	'-Waddress-space',
	'-Wbitwise',
	'-Wcast-to-as',
	'-Wno-pointer-sign',
	'-Wdefault-bitfield-sign',
	'-Wdo-while',
	'-Wparen-string',
	'-Wptr-subtraction-blows',
	'-Wreturn-void',
	'-Wtypesign',
]

# Fine tuning. Some warnings enabled above are a bit too much
# (at least for now)
test_c_args = [
	'-Wno-unused-parameter',
	'-Wno-sign-compare'
]

# Some warnings which we may want to consider later on but which
# trigger too often at the time of writing
not_now_please = [
	'-Wpointer-arith',
	'-Wbad-function-cast',
	'-Wswitch-enum',
	'-Wfloat-equal',
	'-Wdeclaration-after-statement',
	'-Wshadow'
]

foreach arg: test_c_args
	if cc.has_multi_arguments(arg)
		global_c_args += arg
	endif
endforeach

add_project_arguments(
	global_c_args,
	language: 'c'
)

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
subdir('demo')

summary(
	{
		'API Docs': get_option('docs').enabled(),
		'Introspection': get_option('introspection').enabled(),
		'Tests': get_option('tests').enabled(),
		'Demo': get_option('demo').enabled(),
	},
	section: 'Options'
)
