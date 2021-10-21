bison = find_program('bison', required : true)
flex = find_program('flex', required : true)

yyprefix = 'itex2MML_yy'

y_tab = custom_target('bison y.tab.c',
						output : ['y.tab.c', 'y.tab.h'],
						input : 'itex2MML.y',
						command : [bison, '-v', '-p', yyprefix, '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '@INPUT@'])

lex_yy_c = custom_target('flex lex.yy.c',
						 output : 'lex.yy.c',
						 input : 'itex2MML.l',
						 command : [flex, '-P', yyprefix, '-o', '@OUTPUT@', '@INPUT@'])

libitex2mml_sources = [
	'itex2MML.h',
	y_tab,
	lex_yy_c
]

libitex2mml = static_library(
	'itex2mml',
	libitex2mml_sources
)

libitex2mml_dep = declare_dependency(link_with : libitex2mml)
