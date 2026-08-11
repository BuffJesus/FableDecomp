#include <cstdio>

class CIDrawEnvironment;

class CWorld {
public:
    char pad_0x00[0x14];
    CIDrawEnvironment* m_drawEnvironment; // +0x14
    const CIDrawEnvironment* DrawGetEnvironment();
};

const CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
    return m_drawEnvironment;
}

int main()
{
    CWorld w;
    CIDrawEnvironment* sentinel = (CIDrawEnvironment*)0xDEADBEEF;
    w.m_drawEnvironment = sentinel;
    const CIDrawEnvironment* got = w.DrawGetEnvironment();
    if (got == (const CIDrawEnvironment*)0xDEADBEEF) {
        printf("DRAWENV_OK\n");
        return 0;
    }
    printf("DRAWENV_FAIL\n");
    return 1;
}