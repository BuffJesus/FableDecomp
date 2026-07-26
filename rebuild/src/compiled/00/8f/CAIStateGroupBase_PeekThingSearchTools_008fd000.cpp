struct CThingSearchTools;
struct D { char pad[0x20]; CThingSearchTools* p20; };
struct C { char pad[0x24]; D* p24; };
struct B { char pad[0x1c]; C* p1c; };
struct A { char pad[4]; B* p4; };
CThingSearchTools* __fastcall CAIStateGroupBase_PeekThingSearchTools(const A* self)
{
    return self->p4->p1c->p24->p20;
}