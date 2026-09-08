#include <stdio.h>
struct CPersistContext { void* fields[5]; bool flags[4]; long Mode; bool Binary;
 bool IsBinary() const; };
bool CPersistContext::IsBinary() const { return Mode==1 || Mode==3; }
int main() { CPersistContext context;
 context.Mode=0; if(context.IsBinary()) return 1;
 context.Mode=1; if(!context.IsBinary()) return 2;
 context.Mode=2; if(context.IsBinary()) return 3;
 context.Mode=3; if(!context.IsBinary()) return 4;
 printf("PERSIST_CONTEXT_IS_BINARY PASS\n"); return 0; }