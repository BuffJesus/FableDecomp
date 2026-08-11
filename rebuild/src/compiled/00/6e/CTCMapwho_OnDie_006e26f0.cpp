// CTCMapwho::OnDie  0x006e26f0  __fastcall void(CTCMapwho*)
#include <stddef.h>

struct Sub28 {
    int pad;
    void Cleanup();   // 0x6e2500, __fastcall
};

extern "C" void __cdecl free_it(void* p);   // stands in for operator delete 0xbfe9bc

struct CTCMapwho {
    char pad0[0x28];
    Sub28 sub;        // +0x28
    void OnDie();
};

void CTCMapwho::OnDie()
{
    CTCMapwho* self = this;
    if (self != 0) {
        self->sub.Cleanup();
        free_it(self);
    }
}