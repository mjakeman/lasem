# Adapted from glib's tests build file
# https://gitlab.gnome.org/GNOME/glib/-/blob/main/tests/meson.build

if get_option('tests').enabled()

test_deps = [
	deps,
	liblasem_dep
]

test_env = environment()
test_env.set('G_TEST_SRCDIR', meson.current_source_dir())
test_env.set('G_TEST_BUILDDIR', meson.current_build_dir())

tests = {
	'dom' : {},
	'str' : {},
	'filter' : {},
	'suite' : {
	    'c_args': [
	    	'-DSUITE_DATA_DIRECTORY="' + meson.current_source_dir() + '/tests/data"',
	    	'-DSUITE_OPTION_FILE="' + meson.current_source_dir() + '/tests/suite.ini"'
    	],
	}
}

foreach test_name, extra_args : tests

	exe = executable(
		test_name,
		test_name + '.c',
		dependencies: test_deps,
		c_args : extra_args.get('c_args', [])
	)

	test(test_name, exe, env : test_env)

endforeach

# lasem-test program
lasemtest = executable(
    'lsm-test',
    'lsmtest.c',
    dependencies : test_deps
)

endif
