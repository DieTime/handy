cc = meson.get_compiler('c')

deps = [handy_dep]
args = cc.get_supported_arguments('-fanalyzer')

examples = [
    'log',
    'optional',
]

foreach example : examples
    executable(example, f'@example@.c', dependencies: deps, c_args: args)
endforeach
