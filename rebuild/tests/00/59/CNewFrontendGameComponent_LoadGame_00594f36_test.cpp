#include <cstdio>
#include <cstring>

struct FrontEndWideString_00594f36
{
    void* data;
    FrontEndWideString_00594f36& operator=(
        const FrontEndWideString_00594f36& source);
};

class CNewFrontendGameComponent_00594f36
{
public:
    virtual void Slot00();
    virtual void Slot01();
    virtual void Slot02();
    virtual void Slot03();
    virtual void PrepareLoad();

    unsigned char padding_004[0x25];
    unsigned char load_pending;
    unsigned char padding_02a[0x5a];
    FrontEndWideString_00594f36 load_name;

    void LoadGame(const FrontEndWideString_00594f36& name);
};

static int g_Sequence;
static int g_PrepareSequence;
static int g_AssignSequence;
static CNewFrontendGameComponent_00594f36* g_PrepareThis;
static FrontEndWideString_00594f36* g_AssignDestination;
static const FrontEndWideString_00594f36* g_AssignSource;
static unsigned char g_FlagSeenAtPrepare;
static unsigned char g_FlagSeenAtAssign;

extern "C" void __fastcall
FrontEndLoadPrepare_00594f36(
    CNewFrontendGameComponent_00594f36* self,
    void*)
{
    g_PrepareSequence = ++g_Sequence;
    g_PrepareThis = self;
    g_FlagSeenAtPrepare = self->load_pending;
}

FrontEndWideString_00594f36&
FrontEndWideString_00594f36::operator=(
    const FrontEndWideString_00594f36& source)
{
    g_AssignSequence = ++g_Sequence;
    g_AssignDestination = this;
    g_AssignSource = &source;
    g_FlagSeenAtAssign =
        ((CNewFrontendGameComponent_00594f36*)
            ((unsigned char*)this - 0x84))->load_pending;
    data = source.data;
    return *this;
}

static void ResetProbe()
{
    g_Sequence = 0;
    g_PrepareSequence = 0;
    g_AssignSequence = 0;
    g_PrepareThis = 0;
    g_AssignDestination = 0;
    g_AssignSource = 0;
    g_FlagSeenAtPrepare = 0xff;
    g_FlagSeenAtAssign = 0xff;
}

static bool RunLoad(CNewFrontendGameComponent_00594f36* component,
                    FrontEndWideString_00594f36& requested,
                    void** vtable)
{
    *(void***)component = vtable;
    component->load_pending = 0;
    component->load_name.data = (void*)"old";
    requested.data = (void*)"save_slot";
    ResetProbe();

    component->LoadGame(requested);
    return g_PrepareSequence == 1 &&
        g_AssignSequence == 2 &&
        g_PrepareThis == component &&
        g_AssignDestination == &component->load_name &&
        g_AssignSource == &requested &&
        g_FlagSeenAtPrepare == 0 &&
        g_FlagSeenAtAssign == 1 &&
        component->load_pending == 1 &&
        component->load_name.data == requested.data;
}

int main()
{
    unsigned char componentStorage[0x88];
    void* vtable[5] = { 0, 0, 0, 0,
        (void*)&FrontEndLoadPrepare_00594f36 };
    CNewFrontendGameComponent_00594f36* component =
        (CNewFrontendGameComponent_00594f36*)componentStorage;
    FrontEndWideString_00594f36 requested;
    std::memset(componentStorage, 0, sizeof(componentStorage));

    if (!RunLoad(component, requested, vtable))
    {
        std::puts("FRONTEND_00594f36_TEST FAIL");
        return 1;
    }

    std::puts("FRONTEND_00594f36_TEST PASS");
    return 0;
}
