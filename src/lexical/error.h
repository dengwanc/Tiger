// extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int,const char* ,...);
void EM_impossible(char*,...);
void EM_reset(char* filename);
