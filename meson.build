project('lang', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])
incdir = include_directories('include')
src = ['src/lang.c']
exe = executable('lang', src, include_directories: incdir,
  install : true)

test('basic', exe)
