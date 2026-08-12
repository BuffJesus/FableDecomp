#include <stdio.h>

struct CountedPtr {
    void* ptr;
};

struct Alloc {
    int dummy;
};

static Alloc*      g_seen_self = 0;
static CountedPtr* g_seen_p    = 0;
static int         g_calls     = 0;

// definition of the jmp target for the standalone link
void __fastcall Alloc_destroy(Alloc* self, CountedPtr* p)
{
    g_seen_self = self;
    g_seen_p    = p;
    g_calls++;
}

void __fastcall _Dest_val(Alloc* al, CountedPtr* p);

int main()
{
    Alloc      a;      a.dummy = 7;
    CountedPtr cp;     cp.ptr  = (void*)0xABCD;

    _Dest_val(&a, &cp);

    if (g_calls == 1 && g_seen_self == &a && g_seen_p == &cp) {
        printf("DESTVAL_FWD_OK\n");
        return 0;
    }
    printf("FAIL calls=%d\n", g_calls);
    return 1;
}