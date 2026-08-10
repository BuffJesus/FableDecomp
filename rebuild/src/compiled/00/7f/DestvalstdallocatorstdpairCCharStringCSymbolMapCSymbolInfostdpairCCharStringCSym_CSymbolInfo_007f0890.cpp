#pragma optimize("s",on)

struct Pair;

extern "C" void __fastcall Dtor_first(Pair* self);
extern "C" void __fastcall Dtor_second(Pair* self);
extern "C" void __fastcall Dtor_node(Pair* self);

void __fastcall Dest_val(Pair* self)
{
    Dtor_first(self);
    Dtor_second(self);
    Dtor_node(self);
}