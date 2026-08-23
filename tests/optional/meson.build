tests = [
  'defaults',
  'strip_prefix',
]

foreach name : tests
    test(
        f'optional_@name@',
        executable(f'optional_@name@', f'@name@.cpp', dependencies: [handy_dep, gtest_main_dep])
    )
endforeach
