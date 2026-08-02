#include <cstdio>

class CEngine
{
public:
    char pad_0000[0xb8];
    unsigned int flags_0xb8;

    bool __fastcall Is2DSceneRenderingEnabled(void* /*edx*/);
};

bool __fastcall CEngine::Is2DSceneRenderingEnabled(void* /*edx*/)
{
    return (flags_0xb8 >> 1) & 1;
}

int main()
{
    CEngine obj;

    obj.flags_0xb8 = 0;
    bool r1 = obj.Is2DSceneRenderingEnabled(0);
    obj.flags_0xb8 = 2;
    bool r2 = obj.Is2DSceneRenderingEnabled(0);
    obj.flags_0xb8 = 1;
    bool r3 = obj.Is2DSceneRenderingEnabled(0);
    obj.flags_0xb8 = 3;
    bool r4 = obj.Is2DSceneRenderingEnabled(0);
    obj.flags_0xb8 = (unsigned int)~2;
    bool r5 = obj.Is2DSceneRenderingEnabled(0);

    if (r1 == false && r2 == true && r3 == false && r4 == true && r5 == false)
    {
        printf("PASS_IS2DSCENE_TEST\n");
    }
    else
    {
        printf("FAIL r1=%d r2=%d r3=%d r4=%d r5=%d\n", r1, r2, r3, r4, r5);
    }

    return 0;
}