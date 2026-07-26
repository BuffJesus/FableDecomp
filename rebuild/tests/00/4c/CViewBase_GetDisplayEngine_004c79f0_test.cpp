#include <cstdio>
struct CDisplayEngine { int marker; };
struct CViewBase { int dummy; };
static char g_block[0x40];
void* g_displayGlobal = (void*)g_block;

CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(const CViewBase* self)
{
    (void)self;
    return *(CDisplayEngine**)((char*)g_displayGlobal + 0x30);
}

static CDisplayEngine theEngine;
static CDisplayEngine* pEngine = &theEngine;

int main()
{
    theEngine.marker = 0x1234;
    *(CDisplayEngine**)(g_block + 0x30) = pEngine;
    CViewBase v;
    v.dummy = 0;
    CDisplayEngine* r = CViewBase_GetDisplayEngine(&v);
    int ok = (r == pEngine);
    if (ok != 0) {
        if (r->marker == 0x1234) {
            std::printf("CViewBase_004c79f0_TEST PASS\n");
            return 0;
        }
    }
    std::printf("CViewBase_004c79f0_TEST FAIL\n");
    return 1;
}