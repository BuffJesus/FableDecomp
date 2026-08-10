struct Sub { char pad[4]; };

struct Obj {
    char pad[0xA8];
    Sub sub;
};

extern "C" void __fastcall sub_A01C10(Sub* self);
extern "C" void __fastcall sub_693EF0(Obj* self);

void __fastcall DestVal_A8_H(Obj* self)
{
    sub_A01C10(&self->sub);
    sub_693EF0(self);
}