CONFIG -= qt

LIBS += -lfl -ly
QMAKE_CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -DYYDEBUG=1 -DYYERROR_VERBOSE=1 -Wno-implicit-function-declaration

FLEXSOURCES = src/lexer.l
BISONSOURCES = src/parser.y

SOURCES += \
    src/ast/ast.c \
    src/vector.c \
    src/ast/if_node.c \
    src/ast/while_node.c \
    src/ast/program_node.c \
    src/ast/block_node.c \
    src/ast/atom_node.c \
    src/ast/binop_node.c \
    src/ast/unop_node.c \
    src/main.c \
    src/ast/for_node.c \
    src/ast/var_decl_node.c \
    src/ast/ident_node.c \
    src/ast/type_node.c \
    src/ast/fun_decl.c \
    src/ast/fun_call_node.c \
    src/ast/return_node.c \
    src/mid_end/mid_end.c \
    src/mid_end/scope.c \
    src/mid_end/context.c

TEMPLATE = app

flex.commands = flex ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = lex.yy.c
flex.variable_out = SOURCES
flex.depends = ${QMAKE_VAR__PRO_FILE_PWD_}/y.tab.h
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex

bison.commands = bison -v --defines=${QMAKE_VAR__PRO_FILE_PWD_}/y.tab.h -t -y ${QMAKE_FILE_IN} -Wno-other
bison.input = BISONSOURCES
bison.output = y.tab.c
bison.variable_out = SOURCES
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison

bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_VAR__PRO_FILE_PWD_}/y.tab.h
bisonheader.variable_out = HEADERS
bisonheader.name = bison header
bisonheader.depends = y.tab.c
QMAKE_EXTRA_COMPILERS += bisonheader

HEADERS += \
    src/ast/ast.h \
    src/defines.h \
    src/ast/if_node.h \
    src/ast/while_node.h \
    src/ast/program_node.h \
    src/ast/block_node.h \
    src/ast/atom_node.h \
    src/ast/binop_node.h \
    src/ast/unop_node.h \
    src/ast/common.h \
    src/vector.h \
    src/ast/for_node.h \
    src/ast/var_decl_node.h \
    src/ast/ident_node.h \
    src/ast/type_node.h \
    src/ast/fun_decl.h \
    src/ast/fun_call_node.h \
    src/ast/return_node.h \
    src/mid_end/mid_end.h \
    src/mid_end/scope.h \
    src/mid_end/context.h
