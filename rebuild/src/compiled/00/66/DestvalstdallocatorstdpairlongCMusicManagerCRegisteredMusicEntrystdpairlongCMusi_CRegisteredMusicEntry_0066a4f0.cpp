struct Sub;
struct Obj;
extern "C" void __fastcall Sub_Method(Sub* self);
extern "C" void __fastcall Obj_Other(Obj* self);

struct Sub { int x; };
struct Obj {
    char pad[0x114];
    Sub sub;
};

void __fastcall DestVal_Music_B(Obj* self, void* /*edx*/)
{
    Sub_Method(&self->sub);
    Obj_Other(self);
}