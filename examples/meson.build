cc = meson.get_compiler('c')

deps = [ handy_dep ]
args = cc.get_supported_arguments('-fanalyzer')

executable('log', 'log.c', dependencies: deps, c_args: args)
