#include <cstdio>
struct Msg { int type; int a; int b; };
struct Range { Msg* p1; int* p2; };
struct Frame { Msg m; Range r; };

struct Obj { int expect; int Dispatch(Range* r); };
int g_e = 7, g_a = 9;
int __fastcall GetThing1(void){ return g_e; }
int __fastcall GetThing2(void* self){ return g_a; }
int Obj::Dispatch(Range* r){
    if (this->expect != 0x1234) return 0;
    if (r->p1->type != 0x12) return 0;
    if (r->p1->a != 7) return 0;
    if (r->p1->b != 9) return 0;
    if (r->p2 != &r->p1->a) return 0;
    return 1;
}

bool __fastcall CGameScriptInterface_MsgIsGameInfoClickedPast(void* self)
{
    int e = GetThing1();
    int a = GetThing2(self);
    Obj* obj = *(Obj**)((*(int*)((char*)self + 4)) + 0x60);
    Frame f;
    f.r.p1 = &f.m;
    f.m.b = a;
    f.m.a = e;
    f.m.type = 0x12;
    f.r.p2 = &f.m.a;
    return obj->Dispatch(&f.r);
}

int main(){
    static Obj theObj; theObj.expect = 0x1234;
    // block whose +0x60 holds &theObj
    static char block[0x80];
    *(Obj**)(block + 0x60) = &theObj;
    char* pblock = block;
    struct S { int f0; char* f4; } s;
    s.f4 = pblock;
    bool r = CGameScriptInterface_MsgIsGameInfoClickedPast(&s);
    if (r) { std::printf("CGameScriptInterface_00894370_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}