#include <cstdio>
struct CThingSearchTools { int tag; };
struct D { char pad[0x20]; CThingSearchTools* p20; };
struct C { char pad[0x24]; D* p24; };
struct B { char pad[0x1c]; C* p1c; };
struct A { char pad[4]; B* p4; };
CThingSearchTools* __fastcall CAIStateGroupBase_PeekThingSearchTools(const A* self)
{
    return self->p4->p1c->p24->p20;
}
int main(){
    CThingSearchTools t; t.tag=0x1234;
    D d; d.p20=&t;
    C c; c.p24=&d;
    B b; b.p1c=&c;
    A a; a.p4=&b;
    CThingSearchTools* r = CAIStateGroupBase_PeekThingSearchTools(&a);
    if(r==&t && r->tag==0x1234){ std::printf("CAIStateGroupBase_008fd000_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}