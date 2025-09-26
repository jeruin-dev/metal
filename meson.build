project(
  'metal',
  'cpp',
  version: '0.1',
  meson_version: '>= 1.3.0',
  default_options: ['warning_level=3', 'cpp_std=c++23'],
)

llvm_dep = dependency('llvm', modules: ['core', 'irreader'], required: true)

dependencies = [llvm_dep]

executable(
  'metal',
  'metal.cpp',
  'lexer.cpp',
  'parser.cpp',
  'codegen.cpp',
  install: true,
  dependencies: dependencies,
)
