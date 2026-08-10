#include <cstdio>

struct CProceduralTexture;
struct CWorld;

CProceduralTexture const * __fastcall CWorld_DrawGetProceduralTexture(CWorld *self);

int main()
{
    CWorld *w = (CWorld*)0x12345678;
    CProceduralTexture const *r = CWorld_DrawGetProceduralTexture(w);
    if (r == 0)
        std::printf("004ae890_TEST PASS\n");
    else
        std::printf("FAIL %p\n", (const void*)r);
    return 0;
}