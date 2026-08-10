
#include <cstdio>
// behavioural model of the dtor: set vtable, call member dtor, tail base dtor
static int g_member=0, g_base=0; static unsigned g_vt=0;
struct Obj{ unsigned vt; };
void member_dtor(Obj*o){ (void)o; g_member++; }
void base_dtor(Obj*o){ (void)o; g_base++; }
void DtorIPM_H_model(Obj* self){
    self->vt = 0x012c2648u;
    member_dtor(self);
    // tail-call base
    base_dtor(self);
}
int main(){
    Obj o; o.vt=0;
    DtorIPM_H_model(&o);
    int ok = (o.vt==0x012c2648u) && (g_member==1) && (g_base==1);
    // exercise a second run
    DtorIPM_H_model(&o);
    ok = ok && (g_member==2) && (g_base==2);
    if(ok) printf("OK_00cd3ec0\n"); else printf("FAIL\n");
    return ok?0:1;
}