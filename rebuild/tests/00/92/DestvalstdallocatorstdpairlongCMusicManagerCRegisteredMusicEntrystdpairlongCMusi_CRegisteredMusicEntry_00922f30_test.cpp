#include <cstdio>
struct Sub { int x; };
struct T { char pad[0xa8]; Sub sub; };
static int g_sub=0, g_final=0;
void __fastcall SubCall(Sub* s){ g_sub++; }
void __fastcall FinalCall(T* t){ g_final++; }
void __fastcall DestVal_A8_C(T* self){ SubCall(&self->sub); FinalCall(self); }
int main(){
    T t;
    DestVal_A8_C(&t);
    if(g_sub==1 && g_final==1){ puts("OK_00922f30"); return 0; }
    puts("FAIL");
    return 1;
}