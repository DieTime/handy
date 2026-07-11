cc = meson.get_compiler('c')

handy_dep = declare_dependency(
  include_directories: ['.'],
  compile_args: cc.get_supported_arguments('-Wno-gnu-zero-variadic-macro-arguments'),
)
