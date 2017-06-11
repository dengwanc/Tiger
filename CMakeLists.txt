CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

# clang flags
ADD_COMPILE_OPTIONS(-std=c++11)
ADD_COMPILE_OPTIONS(-Wno-deprecated-register)

# lex dep
SET(_LEX_DEP ${CMAKE_SOURCE_DIR}/src/lexical/tiger.lex)
SET(YYLEX ${CMAKE_SOURCE_DIR}/src/lexical/lex.yy.cc)
ADD_CUSTOM_COMMAND(
        MAIN_DEPENDENCY ${_LEX_DEP}
        OUTPUT ${YYLEX}
        COMMAND lex --outfile=${YYLEX} ${_LEX_DEP}
)

# yacc dep
# ~~~
SET(YYTAB ${CMAKE_SOURCE_DIR}/src/syntax/y.tab.cc)

AUX_SOURCE_DIRECTORY(./test TEST)
AUX_SOURCE_DIRECTORY(./src/utils UTILS)
AUX_SOURCE_DIRECTORY(./src/lexical LEXICAL)
AUX_SOURCE_DIRECTORY(./examples EXAMPLES)
AUX_SOURCE_DIRECTORY(./src SOURCE)

# Generate Unit Test Execute File
ADD_EXECUTABLE(test_output
        ${TEST}
        ${UTILS} ${LEXICAL}
        ${YYLEX}
)

# Generate Examples Execute File
ADD_EXECUTABLE(ex_output ${EXAMPLES} ${UTILS})

# Generate Tiger Compiler Execute File
