project('lang', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])
incdir = include_directories('include', 'sds', 'c_arraylist')
src = ['src/lang.c', 'src/lexer.c', 'sds/sds.c', 'c_arraylist/arraylist.c']
exe = executable('lang', src, include_directories: incdir,
  install : true)

test('basic', exe)
