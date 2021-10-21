# Adapted from libadwaita's documentation build file
# https://gitlab.gnome.org/GNOME/libadwaita/-/blob/main/doc/meson.build

if get_option('docs').enabled()

expand_content_md_files = [
	'overview.md'
]

toml_data = configuration_data()
toml_data.set('VERSION', meson.project_version())

lasem_toml = configure_file(
	input: 'lasem.toml.in',
	output: 'lasem.toml',
	configuration: toml_data
)

dependency('gi-docgen', version: '>= 2021.1',
           fallback: ['gi-docgen', 'dummy_dep'],
           native: true,
           required: get_option('docs'))

gidocgen = find_program('gi-docgen')

docs_dir = datadir / 'docs'

custom_target('lasem-doc',
	input: [ lasem_toml, lasem_gir[0] ],
	output: 'lasem-@0@'.format(lasem_api_version),
	command: [
		gidocgen,
		'generate',
		'--quiet',
		'--add-include-path=@0@'.format(meson.current_build_dir() / '../../../src'),
		'--config=@INPUT0@',
		'--output-dir=@OUTPUT@',
		'--no-namespace-dir',
		'--content-dir=@0@'.format(meson.current_source_dir()),
		'@INPUT1@',
	],
	depend_files: [ expand_content_md_files ],
	build_by_default: true,
	install: true,
	install_dir: docs_dir,
)


endif
