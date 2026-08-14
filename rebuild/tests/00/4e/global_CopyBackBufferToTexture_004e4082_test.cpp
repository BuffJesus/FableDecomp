#include <cstdio>

struct CTexture;
struct CDisplayManager;

void __stdcall CopyBackBuffer_FormatStep(bool* a, bool* b);
CTexture* __fastcall CopyBackBufferToTexture(CDisplayManager* self, void*);

static bool* g_a;
static bool* g_b;
static int   g_calls;

void __stdcall CopyBackBuffer_FormatStep(bool* a, bool* b)
{
    g_a = a; g_b = b; ++g_calls;
}

int main()
{
    int obj;
    CDisplayManager* self = (CDisplayManager*)&obj;
    CTexture* r = CopyBackBufferToTexture(self, 0);

    // Forwarder must return `this`, must have called the worker exactly once,
    // and must have handed it two non-null flag addresses.
    if ((void*)r == (void*)self && g_calls == 1 && g_a != 0 && g_b != 0)
        printf("PARITY_OK_004e4082\n");
    else
        printf("PARITY_FAIL r=%p self=%p calls=%d\n", (void*)r, (void*)self, g_calls);
    return 0;
}