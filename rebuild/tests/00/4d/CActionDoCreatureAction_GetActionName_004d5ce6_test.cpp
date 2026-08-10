#include <cstdio>
struct CCharString { const char* v; int n; };
CCharString* ctor(CCharString* self, const char* s, int n){ self->v=s; self->n=n; return self; }
CCharString* model(CCharString* self){ ctor(self,(const char*)0x123d8b0,-1); return self; }
int main(){
    CCharString cs; cs.v=0; cs.n=0;
    CCharString* r=model(&cs);
    if(r!=&cs){ printf("FAIL ret\n"); return 1; }
    if(cs.v!=(const char*)0x123d8b0){ printf("FAIL str\n"); return 1; }
    if(cs.n!=-1){ printf("FAIL n\n"); return 1; }
    printf("OK_004d5ce6\n");
    return 0;
}