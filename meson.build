project('handy', 'c', default_options: ['c_std=gnu11', 'warning_level=3'])

subdir('include')

if get_option('build_examples')
  subdir('examples')
endif

if get_option('build_tests')
  subdir('tests')
endif
