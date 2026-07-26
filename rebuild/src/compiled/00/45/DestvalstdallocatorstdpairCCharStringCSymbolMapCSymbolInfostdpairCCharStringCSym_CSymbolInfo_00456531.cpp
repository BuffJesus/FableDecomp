
struct Sub { char pad[32]; };
struct Owner { char pad[0x50]; };

extern "C" void __fastcall Sub_dtor(Sub* s);
extern "C" void __fastcall Owner_base_dtor(Owner* o);

void __fastcall _Dest_val(Owner* self)
{
    Sub_dtor((Sub*)((char*)self + 0x48));
    Sub_dtor((Sub*)((char*)self + 0x3c));
    Owner_base_dtor(self);
}