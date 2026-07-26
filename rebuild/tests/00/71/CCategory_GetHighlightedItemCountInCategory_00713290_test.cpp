#include <cstdio>

struct CResult {
    void* got;
    void Init(void* p) { got = p; }
};

struct CCategory {
    char pad[0x30];
    char* base;
    char pad2[8];
    int count;
};

long __fastcall CCategory_GetHighlightedItemCountInCategory(CCategory* self, void* edx, CResult* out)
{
    (void)edx;
    void* p = (void*)(self->base + self->count * 0x44 + 0xc);
    out->Init(p);
    return (long)out;
}

int main()
{
    CCategory c;
    c.base = (char*)0x1000;
    c.count = 2;
    CResult r;
    r.got = 0;
    long ret = CCategory_GetHighlightedItemCountInCategory(&c, 0, &r);
    void* expect = (void*)(0x1000 + 2 * 0x44 + 0xc);
    if (ret == (long)&r && r.got == expect) {
        std::printf("CCategory_00713290_TEST PASS\n");
        return 0;
    }
    std::printf("CCategory_00713290_TEST FAIL\n");
    return 1;
}