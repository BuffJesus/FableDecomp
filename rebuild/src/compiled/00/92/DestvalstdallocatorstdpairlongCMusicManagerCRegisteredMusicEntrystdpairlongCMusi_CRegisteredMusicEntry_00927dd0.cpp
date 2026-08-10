struct Sub { char pad[4]; };
struct Obj {
    char pad[0xa8];
    Sub sub_a8;
};

extern "C" void __fastcall helper_a01c10(Sub* s);
extern "C" void __fastcall tail_693ef0(Obj* self);

void __fastcall DestVal_A8_D(Obj* self, void* /*edx*/)
{
    helper_a01c10(&self->sub_a8);
    tail_693ef0(self);
}