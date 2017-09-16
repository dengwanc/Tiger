#ifndef TIGER_LEXICAL
#define TIGER_LEXICAL

/**
 * This module's responsibility
 * is for parse token & output
 * file, line, char position
 * information.
 *
 * Thus Error Report also
 * in this module, get
 * line number is convenient.
 */

namespace lexical {
    int getLine();
    int getOffset();
    const char* getFilename();

    void adjust();
    void newline();

    void initString();
    void appendChar(int ch);
    void appendStr(char *s);
    void endString();

    void inc();
    void dec();
    bool hasComment();

    void recordString();
    void recordId();
    void recordInt();
    void recordReal();

    void reset(const char *path);
    void parse(const char* path/*, std::function<void()> reject*/);
}

struct Location {
    int line;
    int offset;
};

bool hasErrors();
void reportError(const char* message);
void reportBadToken();
void clearErrors();
void yyerror(const char*);
void reportSemanticError(char* msg, struct Location &lo);

#endif
