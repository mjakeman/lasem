if get_option('demo').enabled()

demo_deps = [
	deps,
	liblasem_dep,
	dependency('gtk4')
]

lasemdemo = executable(
    'lsm-demo',
    'demo.c',
    dependencies : demo_deps,
    install : true
)

endif
