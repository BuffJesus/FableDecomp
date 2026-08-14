#include <stdio.h>

class CTexture;
class CDisplayManager { public: int marker; };

void __stdcall CopyBackBuffer_Blit(char *dst, char *src)
{
    *dst = 1;
    *src = 2;
}

CTexture * __fastcall CopyBackBufferToTexture(CDisplayManager *self);

int main()
{
    CDisplayManager dm;
    dm.marker = 0x1234;
    CTexture *r = CopyBackBufferToTexture(&dm);
    if ((void *)r == (void *)&dm)
        printf("PASS_CopyBackBufferToTexture\n");
    else
        printf("FAIL\n");
    return 0;
}