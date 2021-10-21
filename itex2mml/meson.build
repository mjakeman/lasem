bison = find_program('bison', required : true)
flex = find_program('flex', required : true)

yyprefix = 'itex2MML_yy'

y_tab_c = custom_target('bison y.tab.c',
						output : 'y.tab.c',
						input : 'itex2MML.y',
						command : [bison, '-v', '-p', yyprefix, '-d', '@INPUT@', '-o', '@OUTPUT@'])

lex_yy_c = custom_target('flex lex.yy.c',
						 output : 'lex.yy.c',
						 input : 'itex2MML.l',
						 command : [flex, '-P', yyprefix, '-o', '@OUTPUT@', '@INPUT@'])

libitex2mml_sources = [
	'itex2MML.h',
	y_tab_c,
	lex_yy_c
]

libitex2mml = static_library(
	'itex2mml',
	libitex2mml_sources
)

libitex2mml_dep = declare_dependency(link_with : libitex2mml)
