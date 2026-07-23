#include <cstdio>

struct C3DAnimationInfo { int tag; };

static int g_loadCalled = 0;
static void* g_loadSelf = 0;
static int g_loadArg = 0;
static C3DAnimationInfo g_result;

struct CBank {
    int handle;
    void Load(int arg)
    {
        g_loadCalled = 1;
        g_loadSelf = this;
        g_loadArg = arg;
    }
};

struct CEngine {
    char pad[0xc0];
    CBank* banks; // +0xc0
    C3DAnimationInfo* Resolve()
    {
        return &g_result;
    }
};

C3DAnimationInfo* __fastcall CEngine_GetEditorAnimation(CEngine* self, int dummy, int idx, int arg)
{
    self->banks[idx].Load(arg);
    return self->Resolve();
}

int main()
{
    CEngine e;
    CBank banks[8];
    e.banks = banks;
    g_result.tag = 0x1234;

    C3DAnimationInfo* r = CEngine_GetEditorAnimation(&e, 0, 3, 99);

    if (!g_loadCalled) { std::printf("FAIL load not called\n"); return 1; }
    if (g_loadSelf != (void*)&banks[3]) { std::printf("FAIL load self offset\n"); return 1; }
    if (g_loadArg != 99) { std::printf("FAIL load arg\n"); return 1; }
    if (r != &g_result) { std::printf("FAIL result\n"); return 1; }

    std::printf("CEngine_0040bc50_TEST PASS\n");
    return 0;
}