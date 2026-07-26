#include <cstdio>

static int g_calls = 0;
struct ULSlot {
    void Assign(unsigned long val);
};
static unsigned long g_store[8];
void ULSlot::Assign(unsigned long val) {
    ++g_calls;
    *((unsigned long*)this) = val;
}

unsigned long* __fastcall Fill_n(unsigned long* first, unsigned int count, unsigned long val)
{
    for (; count > 0; --count, ++first)
    {
        ULSlot* p = (ULSlot*)first;
        if (p)
            p->Assign(val);
    }
    return first;
}

int main() {
    for (int i = 0; i < 8; ++i) g_store[i] = 0;
    unsigned long* ret = Fill_n(g_store, 5, 0xABCDu);
    if (ret != g_store + 5) { std::printf("FAIL ret\n"); return 1; }
    if (g_calls != 5) { std::printf("FAIL calls %d\n", g_calls); return 1; }
    for (int i = 0; i < 5; ++i) if (g_store[i] != 0xABCDu) { std::printf("FAIL val %d\n", i); return 1; }
    if (g_store[5] != 0) { std::printf("FAIL overrun\n"); return 1; }
    std::printf("_global_0040f5b0_TEST PASS\n");
    return 0;
}