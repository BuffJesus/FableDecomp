#include <cstdio>

struct Sub { int v; };
struct Def { char pad[0x3c]; Sub sub; };
struct Src { char bytes[0x100]; };

static int g_base_called = 0;
static int g_sub_called = 0;
static const void* g_base_src = 0;
static const void* g_sub_src = 0;

void BaseCopy(Def* self, void const* src){ g_base_called=1; g_base_src=src; (void)self; }
void SubCopy(Sub* self, void const* src){ g_sub_called=1; g_sub_src=src; (void)self; }

void Copy(Def* self, void const* src)
{
    BaseCopy(self, src);
    SubCopy(&self->sub, (char const*)src + 0x3c);
}

int main(){
    Def d; Src s;
    Copy(&d, &s);
    bool ok = g_base_called && g_sub_called
        && g_base_src == (void const*)&s
        && g_sub_src == (void const*)((char const*)&s + 0x3c);
    if(ok) printf("BEHAVIOR_PASS\n");
    else printf("BEHAVIOR_FAIL b=%d s=%d\n", g_base_called, g_sub_called);
    return 0;
}