#ifndef TIGER_LEXICAL
#define TIGER_LEXICAL

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

#endif
