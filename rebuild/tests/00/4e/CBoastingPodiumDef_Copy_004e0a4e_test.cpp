#include <cstdio>

struct CDefClassBase {
    int pad[10];
    unsigned int field28; // 0x28
    void Copy(const CDefClassBase* src);
};

struct CBoastingPodiumDef : CDefClassBase {
    void Copy(const CDefClassBase* src);
};

static int g_baseCalled = 0;
void CDefClassBase::Copy(const CDefClassBase* src)
{
    g_baseCalled = 1;
    (void)src;
}

void CBoastingPodiumDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::Copy(src);
    this->field28 = src->field28;
}

int main()
{
    CBoastingPodiumDef dst;
    CDefClassBase src;
    dst.field28 = 0;
    src.field28 = 0xDEADBEEF;
    dst.Copy(&src);
    if (g_baseCalled == 1 && dst.field28 == 0xDEADBEEF)
        printf("004e0a4e_TEST PASS\n");
    else
        printf("FAIL base=%d f28=%08x\n", g_baseCalled, dst.field28);
    return 0;
}