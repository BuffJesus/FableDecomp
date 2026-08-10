// forward-declared external targets (relocation-masked)
struct Sub { char pad[4]; };
struct Base { char pad[4]; };

extern "C" void __fastcall Sub_dtor(Sub* s);   // 0xa01c10
extern "C" void __fastcall Base_dtor(Base* b); // 0x693ef0

struct Obj {
    char pad_a8[0xa8];
    Sub  sub;      // at +0xa8
};

void __fastcall DestVal_Music_E(Obj* self, void* /*edx*/)
{
    Sub_dtor(&self->sub);
    Base_dtor((Base*)self);
}