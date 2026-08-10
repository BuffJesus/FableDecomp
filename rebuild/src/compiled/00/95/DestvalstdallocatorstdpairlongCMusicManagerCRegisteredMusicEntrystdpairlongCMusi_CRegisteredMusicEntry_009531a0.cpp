struct Sub { char pad[4]; };
struct Outer { char pad[0x114]; Sub member; };

extern "C" void __fastcall sub_dtor(void* self);
extern "C" void __fastcall base_dtor(void* self);

void __fastcall DestVal_114_D(void* self, void* /*edx*/)
{
    Outer* p = (Outer*)self;
    sub_dtor(&p->member);
    base_dtor(self);
}