#include <cstdio>

static int g_base = 0;
static int g_sub = 0;
static void* g_base_this = 0;
static const void* g_base_src = 0;
static void* g_sub_this = 0;
static const void* g_sub_src = 0;

struct CHitLocationsDef {
    unsigned char _tail[0x34];
    void BaseCopy(const CHitLocationsDef* src);
};
struct CSub {
    void SubCopy(const CSub* src);
};

void CHitLocationsDef::BaseCopy(const CHitLocationsDef* src) { g_base++; g_base_this = this; g_base_src = src; }
void CSub::SubCopy(const CSub* src) { g_sub++; g_sub_this = this; g_sub_src = src; }

void __fastcall CHitLocationsDef_Copy(CHitLocationsDef* self, int /*edx*/, const CHitLocationsDef* src)
{
    self->BaseCopy(src);
    ((CSub*)((unsigned char*)self + 0x28))->SubCopy((const CSub*)((const unsigned char*)src + 0x28));
}

int main() {
    CHitLocationsDef dst, s;
    CHitLocationsDef_Copy(&dst, 0, &s);
    if (g_base != 1 || g_sub != 1) { std::printf("FAIL count\n"); return 1; }
    if (g_base_this != (void*)&dst || g_base_src != (const void*)&s) { std::printf("FAIL base args\n"); return 1; }
    if (g_sub_this != (void*)((unsigned char*)&dst + 0x28) || g_sub_src != (const void*)((const unsigned char*)&s + 0x28)) { std::printf("FAIL sub args\n"); return 1; }
    std::printf("CHitLocationsDef_00454993_TEST PASS\n");
    return 0;
}