struct Sub { char pad[4]; };
struct Obj {
    char pad[0xa8];
    Sub sub;
};

extern "C" void __fastcall helper_a01c10(void* p);
extern "C" void __fastcall helper_693ef0(void* p);

extern "C" void __fastcall DestVal_A8_K(void* self, void* /*edx*/)
{
    Obj* o = (Obj*)self;
    helper_a01c10(&o->sub);
    helper_693ef0(o);
}