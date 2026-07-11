tests = [
  'format',
  'level_filter',
  'level_filter_error',
  'level_filter_none',
  'defaults',
  'stdio_target',
  'strip_prefix',
  'flush_all',
  'flush_errors',
  'flush_none',
  'thread_safety',
  'colored_output',
]

foreach name : tests
    test(
        f'log_@name@',
        executable(f'log_@name@', f'@name@.cpp', dependencies: [handy_dep, gtest_main_dep])
    )
endforeach
