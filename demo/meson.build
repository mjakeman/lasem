if get_option('demo').enabled()

demo_deps = [
	deps,
	liblasem_dep,
	dependency('gtk4')
]

demo_resources = gnome.compile_resources(
    'demo-resources', 'demo-resources.gresource.xml',
    c_name: 'demo_resources'
)

lasemdemo = executable(
    'lsm-demo',
    'demo.c',
    demo_resources,
    dependencies : demo_deps,
    install : true
)

endif
