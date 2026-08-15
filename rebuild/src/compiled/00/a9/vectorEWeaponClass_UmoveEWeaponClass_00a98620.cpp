// _Umove for vector<EWeaponClass> : returns EWeaponClass*
// retail 0xa98620: __fastcall (ecx=self, ONE stack arg p). ret 4.
//   sub esp,8; edx=[esp+0xc]=p; zero 8-byte local; push &local; push p; call 0xa985a0; add esp,8; ret 4
// Model as __fastcall with a DUMMY edx param so the pointer args live on the STACK (ecx-only = thiscall).
typedef int EWeaponClass;

struct Pair { int a; int b; };
struct Vec;

// callee 0xa985a0 : ecx=self, stack args (p, q) -> EWeaponClass*
EWeaponClass* __fastcall helper(Vec* self, int /*edx*/, EWeaponClass* p, Pair* q);

// the target function
EWeaponClass* __fastcall Umove(Vec* self, int /*edx*/, EWeaponClass* p)
{
    Pair local;
    local.a = 0;
    local.b = 0;
    return helper(self, (int)p, p, &local);
}