#include <cstdio>

static int g_inner = 0;
static int g_pair = 0;
static void* g_last_inner = 0;
static void* g_last_pair = 0;

struct Inner { void dtor(); };
struct Pair { char pad[0x39]; Inner inner; void dtor(); };

void Inner::dtor() { g_inner++; g_last_inner = this; }
void Pair::dtor() { g_pair++; g_last_pair = this; }

void __fastcall Dest_val(Pair* self)
{
    self->inner.dtor();
    self->dtor();
}

int main()
{
    Pair p;
    Dest_val(&p);
    if (g_inner == 1 && g_pair == 1 &&
        g_last_inner == (void*)((char*)&p + 0x39) &&
        g_last_pair == (void*)&p) {
        std::printf("_Dest_val_00454b4c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL inner=%d pair=%d\n", g_inner, g_pair);
    return 1;
}