CONFIG += console
CONFIG -= app_bundle

LIBS += -lfl -ly
QMAKE_CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -DYYDEBUG=1 -DYYERROR_VERBOSE=1

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
    src/main.c

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
    src/vector.h
