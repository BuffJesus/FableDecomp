#include <cstdio>

struct CDefClassBase;

struct CEnemyDef {
    char pad[0x28];
    int f28;
    void base_clone(const CDefClassBase* src);
};

static void* g_self;
static const void* g_src;

void CEnemyDef::base_clone(const CDefClassBase* src)
{
    g_self = this;
    g_src = src;
}

void __fastcall CEnemyDef_Copy(CEnemyDef* self, int /*edx*/, const CDefClassBase* src);

int main()
{
    CEnemyDef dst; dst.f28 = 0;
    CEnemyDef srcObj; srcObj.f28 = 0x12345678;
    CEnemyDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.f28 == 0x12345678 && g_self == &dst && g_src == (const void*)&srcObj)
        printf("004e064e_TEST PASS\n");
    else
        printf("FAIL %x\n", dst.f28);
    return 0;
}