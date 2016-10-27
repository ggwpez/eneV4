CONFIG += console
CONFIG -= app_bundle

LIBS += -lfl -ly
QMAKE_CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -DYYDEBUG=1 -DYYERROR_VERBOSE=1

FLEXSOURCES = lexer.l
BISONSOURCES = parser.y

SOURCES += \
    main.cpp \
    ast.c \
    vector.c

TEMPLATE = app

flex.commands = flex ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = lex.yy.c
flex.variable_out = SOURCES
flex.depends = ${QMAKE_VAR__PRO_FILE_PWD_}/y.tab.h
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex

bison.commands = bison --defines=${QMAKE_VAR__PRO_FILE_PWD_}/y.tab.h -t -y ${QMAKE_FILE_IN} -Wno-other
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
    ast.h \
    defines.h
