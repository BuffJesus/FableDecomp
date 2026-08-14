#include <cstdio>

struct EWeaponClass { int v; };
struct Alloc { int tag; };
struct VecBase { void* _m0; Alloc* _al; };

static Alloc*        g_seen_this;
static EWeaponClass* g_seen_first;
static unsigned int  g_seen_marker;
static EWeaponClass  g_ret;

EWeaponClass* __fastcall helper(Alloc* self, void* /*edx*/, EWeaponClass* first, unsigned int* marker)
{
    g_seen_this  = self;
    g_seen_first = first;
    g_seen_marker = *marker;
    return &g_ret;
}

EWeaponClass* __fastcall umove(VecBase* self, void* /*edx*/, EWeaponClass* first)
{
    unsigned int marker = 0xffffffff;
    return helper(self->_al, 0, first, &marker);
}

int main()
{
    Alloc al; al.tag = 55;
    VecBase v; v._m0 = 0; v._al = &al;
    EWeaponClass a;
    EWeaponClass* r = umove(&v, 0, &a);
    if (r == &g_ret &&
        g_seen_this == &al &&
        g_seen_first == &a &&
        g_seen_marker == 0xffffffffu) {
        printf("UMOVE_OK\n");
    } else {
        printf("UMOVE_FAIL this=%p first=%p marker=%08x\n",
               (void*)g_seen_this, (void*)g_seen_first, g_seen_marker);
    }
    return 0;
}