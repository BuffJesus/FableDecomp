#include <stdio.h>
struct S { char pad[0x14]; unsigned char m14; unsigned char m15; };
static bool gRet;
static bool check(S* s){ return gRet; }
void model(S* s){ if(check(s)){ s->m14=1; s->m15=1; } }
int main(){
    S a; a.m14=0; a.m15=0;
    gRet=false; model(&a);
    if(a.m14!=0||a.m15!=0){ printf("FAIL false branch\n"); return 1; }
    gRet=true; model(&a);
    if(a.m14!=1||a.m15!=1){ printf("FAIL true branch\n"); return 1; }
    printf("OK_00789fd0\n");
    return 0;
}