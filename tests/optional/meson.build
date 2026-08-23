tests = [
    'defaults',
    'strip_prefix',
]

args = [
    '-Wno-c++20-extensions',
    '-Wno-pedantic',
    '-Wno-missing-field-initializers',
]

foreach name : tests
    test(
        f'optional_@name@',
        executable(
            f'optional_@name@',
            f'@name@.cpp',
            dependencies: [handy_dep, gtest_main_dep],
            cpp_args: args,
        ),
    )
endforeach
