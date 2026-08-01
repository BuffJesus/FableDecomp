#include <stdio.h>

// Mirror the reconstruction's layout so the behavior test drives the same
// member offsets the retail code uses.

struct Sink {
    int lastTaken;
    int takeCount;
    void Take(int v);
};

struct Local {
    unsigned char flag;
    void Fill(int a, void* b);
};

class CEngineLightingManager {
public:
    char  pad00[0x18];
    int   mode;                  // +0x18
    char  pad1C[0x08];
    int   obj24;                 // +0x24
    Sink* sink28;                // +0x28
    void UpdateShadowScene(unsigned char* p);
};

// --- stubs for masked callees so the test links stand-alone ---
static int g_prepCalls = 0;
static unsigned int g_prepTag = 0;
void __stdcall Prep(unsigned int tag) { ++g_prepCalls; g_prepTag = tag; }

static int g_fillCalls = 0;
static int g_fillA = 0;
static void* g_fillB = 0;
void Local::Fill(int a, void* b) { ++g_fillCalls; g_fillA = a; g_fillB = b; }

void Sink::Take(int v) { lastTaken = v; ++takeCount; }

// The reconstruction defines this out-of-line.
// (When linking source.obj + test.obj this stub is unused; when linking the
// test alone we still need a definition, so provide a faithful reference one.)
#ifdef FABLE_TEST_STANDALONE
void CEngineLightingManager::UpdateShadowScene(unsigned char* p)
{
    Local h;
    Prep(0x122d70eU);
    h.flag = 0;
    switch (this->mode) {
    case 2:  h.Fill(this->obj24, p); break;
    case 3:  this->sink28->Take(*p); break;
    }
}
#endif

int main(void)
{
    Sink sink;
    sink.lastTaken = -1;
    sink.takeCount = 0;

    CEngineLightingManager mgr;
    mgr.obj24 = 0x1234;
    mgr.sink28 = &sink;

    unsigned char byteVal = 0xAB;

    // case 3: sink28->Take((unsigned char)*p)
    mgr.mode = 3;
    g_prepCalls = 0;
    mgr.UpdateShadowScene(&byteVal);
    int ok3 = (g_prepCalls == 1) && (g_prepTag == 0x122d70eU) &&
              (sink.takeCount == 1) && (sink.lastTaken == 0xAB);

    // case 2: local.Fill(this->obj24, p)
    mgr.mode = 2;
    g_prepCalls = 0;
    g_fillCalls = 0;
    mgr.UpdateShadowScene(&byteVal);
    int ok2 = (g_prepCalls == 1) && (g_fillCalls == 1) &&
              (g_fillA == 0x1234) && (g_fillB == (void*)&byteVal);

    // default: neither sink nor fill fires (Prep still runs)
    mgr.mode = 7;
    g_prepCalls = 0;
    g_fillCalls = 0;
    int prevTake = sink.takeCount;
    mgr.UpdateShadowScene(&byteVal);
    int okD = (g_prepCalls == 1) && (g_fillCalls == 0) &&
              (sink.takeCount == prevTake);

    if (ok3 && ok2 && okD)
        printf("UPDATE_SHADOW_SCENE_SWITCH_TEST PASS\n");
    else
        printf("FAIL ok3=%d ok2=%d okD=%d\n", ok3, ok2, okD);
    return 0;
}