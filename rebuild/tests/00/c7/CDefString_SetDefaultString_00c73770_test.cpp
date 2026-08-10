#include <stdio.h>
unsigned char g_flag;
struct S { int calls; };
S* helper(S* self){ self->calls++; return self; }
S* model(S* self){
    if(!g_flag) helper(self);
    return self;
}
int main(){
    S a; a.calls=0;
    g_flag=0;
    S* r = model(&a);
    if(r!=&a || a.calls!=1){ printf("FAIL1\n"); return 1; }
    g_flag=1;
    S* r2 = model(&a);
    if(r2!=&a || a.calls!=1){ printf("FAIL2\n"); return 1; }
    printf("OK_00c73770\n");
    return 0;
}