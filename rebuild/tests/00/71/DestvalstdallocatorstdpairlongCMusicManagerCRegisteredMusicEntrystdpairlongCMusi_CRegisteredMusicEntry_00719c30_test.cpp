#include <cstdio>
struct Sub { int pad; };
struct Obj { char pad[0x114]; Sub sub; };
static int g_sub=0, g_fin=0; static Sub* g_subptr=0; static Obj* g_objptr=0;
extern "C" void __fastcall SubHelper(Sub* s){ g_sub++; g_subptr=s; }
extern "C" void __fastcall FinishHelper(Obj* o){ g_fin++; g_objptr=o; }
void __fastcall DestVal_Music_D(Obj* self);
int main(){
    Obj o;
    DestVal_Music_D(&o);
    int ok=1;
    if(g_sub!=1) ok=0;
    if(g_fin!=1) ok=0;
    if(g_subptr!=&o.sub) ok=0;
    if(g_objptr!=&o) ok=0;
    if((char*)&o.sub-(char*)&o!=0x114) ok=0;
    if(ok) printf("OK_00719c30\n"); else printf("FAIL\n");
    return ok?0:1;
}