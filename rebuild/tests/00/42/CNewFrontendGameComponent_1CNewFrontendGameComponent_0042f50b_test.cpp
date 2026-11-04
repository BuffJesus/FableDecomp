#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "test_support/base_class_stubs.h"

// Fixture for ~CNewFrontendGameComponent 0x0042F50B and its scalar deleting
// destructor 0x004330D2. Every observable step is logged as (kind, pointer).
namespace
{
    enum Kind
    {
        EV_SHUTDOWN = 1,
        EV_WIDE,
        EV_CHAR,
        EV_DELETE_DATA,
        EV_DEF_RELEASED,
        EV_BASE,
        EV_OP_DELETE
    };

    struct Event
    {
        int kind;
        const void* ptr;
        const void* vptr;
    };

    Event g_log[64];
    int g_count;
    const void* g_watchDelete;

    void Log(int kind, const void* ptr, const void* vptr = 0)
    {
        if (g_count < 64)
        {
            g_log[g_count].kind = kind;
            g_log[g_count].ptr = ptr;
            g_log[g_count].vptr = vptr;
        }
        ++g_count;
    }

    void FABLE_FASTCALL DeleteRecorder(void* data)
    {
        Log(EV_DELETE_DATA, data);
    }

    class TestDef : public CDefPointeeBase
    {
    public:
        virtual void DeleteCIVCountedPointeeBase() const { Log(EV_DEF_RELEASED, this); }
    };

    unsigned long Dword(const unsigned char* p, unsigned long offset)
    {
        unsigned long value;
        memcpy(&value, p + offset, sizeof(value));
        return value;
    }

    void PutDword(unsigned char* p, unsigned long offset, const void* value)
    {
        memcpy(p + offset, &value, sizeof(value));
    }

    bool Expect(int index, int kind, const void* ptr)
    {
        return index < g_count && index < 64 &&
            g_log[index].kind == kind && g_log[index].ptr == ptr;
    }
}

void operator delete(void* block)
{
    if (block != 0 && block == g_watchDelete)
        Log(EV_OP_DELETE, block);
    free(block);
}

// Recorders for the real callees.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    Log(EV_BASE, this, *reinterpret_cast<void* const*>(this));
}
// The def base is CResource (retail); its out-of-line pieces.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
CResource::CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) {}
void CResourceList::RemoveResource(CResource&) {}
void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
CGameComponent::CGameComponent(CGame& game) { Game = &game; Quit = false; Running = false; }
void CGameComponent::Init() {}
void CGameComponent::SetQuit() {}
void CNewFrontendGameComponent::Init() {}
bool CNewFrontendGameComponent::Run(CGameComponent**) { return false; }
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::SetQuit() {}
void CNewFrontendGameComponent::OnPreDeviceReset() {}
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }
void CNewFrontendGameComponent::Shutdown() { Log(EV_SHUTDOWN, this); }
double FABLE_FASTCALL GFGetTime() { return 0.0; }
CStopWatch::CStopWatch() {}
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t*) : storage_(0) {}
CWideString::~CWideString() { Log(EV_WIDE, this); }
const CWideString& CWideString::operator=(const wchar_t*) { return *this; }
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() { Log(EV_CHAR, this); }

// Link stand-in: the real constructor is 0x0042EA8F (its own catalog entry).
CNewFrontendGameComponent::CNewFrontendGameComponent(
    CGame& game, const CNewFrontendGameComponentInit&)
    : CGameComponent(game)
{
}

namespace
{
    unsigned char g_game[sizeof(CGame)];
    int g_data[6];
    TestDef g_def;

    // Counted pointer slots, in declaration order.
    const unsigned long kCounted[6] = { 0x10, 0x40, 0x48, 0x50, 0x58, 0x60 };

    // FileNames gets two raw elements; both vector buffers are malloc'd
    // because the destructor frees them.
    CNewFrontendGameComponent* Build(void* storage, bool quit, unsigned long refCount,
        CWideString** names)
    {
        CNewFrontendGameComponentInit init;
        init.Nothing = 0;
        CNewFrontendGameComponent* c = new (storage)
            CNewFrontendGameComponent(*reinterpret_cast<CGame*>(g_game), init);
        unsigned char* p = static_cast<unsigned char*>(storage);
        p[8] = quit ? 1 : 0;
        for (int i = 0; i < 6; ++i)
        {
            CCPPointerInfo* info = new CCPPointerInfo(DeleteRecorder, &g_data[i]);
            info->RefCount = refCount;
            PutDword(p, kCounted[i], &g_data[i]);
            PutDword(p, kCounted[i] + 4, info);
        }
        g_def.IVRefCount = (long)refCount;
        PutDword(p, 0x3C, &g_def);
        *names = static_cast<CWideString*>(malloc(3 * sizeof(CWideString)));
        memset(*names, 0, 3 * sizeof(CWideString));
        PutDword(p, 0x6C, *names);
        PutDword(p, 0x70, *names + 2);
        PutDword(p, 0x74, *names + 3);
        char* times = static_cast<char*>(malloc(0x1C * 2));
        PutDword(p, 0x78, times);
        PutDword(p, 0x7C, times + 0x1C);
        PutDword(p, 0x80, times + 0x38);
        PutDword(p, 0x68, (void*)0x12345678);   // m_pInputManager: untouched
        return c;
    }

    // The full member teardown after the optional Shutdown, from `first`.
    bool Teardown(const unsigned char* p, int first, const CWideString* names)
    {
        int i = first;
        return
            Expect(i++, EV_WIDE, p + 0x84) &&            // SaveGame
            Expect(i++, EV_WIDE, names) &&               // FileNames[0]
            Expect(i++, EV_WIDE, names + 1) &&           // FileNames[1]
            Expect(i++, EV_DELETE_DATA, &g_data[5]) &&   // m_pTextBank
            Expect(i++, EV_DELETE_DATA, &g_data[4]) &&   // m_pEngine
            Expect(i++, EV_DELETE_DATA, &g_data[3]) &&   // PSampleBank
            Expect(i++, EV_DELETE_DATA, &g_data[2]) &&   // m_pMeshBank
            Expect(i++, EV_DELETE_DATA, &g_data[1]) &&   // m_pFrontEndGraphicBank
            Expect(i++, EV_DEF_RELEASED, &g_def) &&      // m_pFrontEndDef
            Expect(i++, EV_WIDE, p + 0x38) &&            // m_bInitMainGame.SaveGameName
            Expect(i++, EV_CHAR, p + 0x34) &&            // .InitialQuestName
            Expect(i++, EV_WIDE, p + 0x30) &&            // .InitialWorldHolySiteName
            Expect(i++, EV_WIDE, p + 0x2C) &&            // .InitialWorldName
            Expect(i++, EV_DELETE_DATA, &g_data[0]) &&   // m_pMenuFont
            Expect(i++, EV_BASE, p);
    }

    bool Cleared(const unsigned char* p)
    {
        for (int i = 0; i < 6; ++i)
            if (Dword(p, kCounted[i]) != 0 || Dword(p, kCounted[i] + 4) != 0)
                return false;
        return Dword(p, 0x3C) == 0 && Dword(p, 0x68) == 0x12345678;
    }
}

int main()
{
    const unsigned long size = sizeof(CNewFrontendGameComponent);
    CWideString* names;

    // 1. Not quit, last references: Shutdown first, then every member in
    //    reverse order, then the CGameComponent vptrs before ~CBaseClassNonCopyable.
    unsigned char* a = static_cast<unsigned char*>(malloc(size));
    memset(a, 0xA5, size);
    CNewFrontendGameComponent* c = Build(a, false, 1, &names);
    const unsigned long derivedVptr = Dword(a, 0);
    const unsigned long derivedVptr2 = Dword(a, 4);
    g_count = 0;
    c->CNewFrontendGameComponent::~CNewFrontendGameComponent();
    const bool first =
        g_count == 16 && Expect(0, EV_SHUTDOWN, a) && Teardown(a, 1, names) && Cleared(a) &&
        // ~CBaseClassNonCopyable rewrites +0 itself; +4 still shows the
        // CGameComponent secondary vptr the destructor stored before it.
        Dword(a, 0) != derivedVptr && Dword(a, 4) != derivedVptr2 && Dword(a, 4) != 0 &&
        g_def.IVRefCount == 0;
    free(a);

    // 2. Already quit, shared references: no Shutdown, no deletes, counts drop.
    unsigned char* b = static_cast<unsigned char*>(malloc(size));
    memset(b, 0xA5, size);
    CCPPointerInfo* infos[6];
    c = Build(b, true, 2, &names);
    for (int i = 0; i < 6; ++i)
        infos[i] = reinterpret_cast<CCPPointerInfo*>(Dword(b, kCounted[i] + 4));
    g_count = 0;
    c->CNewFrontendGameComponent::~CNewFrontendGameComponent();
    bool shared = Cleared(b) && g_def.IVRefCount == 1 && g_count == 8 &&
        Expect(0, EV_WIDE, b + 0x84) && Expect(1, EV_WIDE, names) && Expect(2, EV_WIDE, names + 1) &&
        Expect(3, EV_WIDE, b + 0x38) && Expect(4, EV_CHAR, b + 0x34) &&
        Expect(5, EV_WIDE, b + 0x30) && Expect(6, EV_WIDE, b + 0x2C) && Expect(7, EV_BASE, b);
    for (int i = 0; i < 6; ++i)
    {
        shared = shared && infos[i]->RefCount == 1;
        delete infos[i];
    }
    free(b);

    // 3. Scalar deleting destructor, flag 0 (explicit virtual destructor
    //    call): destroys but does not free.
    unsigned char* d = static_cast<unsigned char*>(malloc(size));
    memset(d, 0xA5, size);
    c = Build(d, false, 1, &names);
    g_count = 0;
    g_watchDelete = d;
    c->~CNewFrontendGameComponent();
    g_watchDelete = 0;
    const bool noFree = g_count == 16 && Expect(0, EV_SHUTDOWN, d) && Teardown(d, 1, names);
    free(d);

    // 4. Scalar deleting destructor, flag 1 (delete through the base):
    //    destroys, then frees this.
    void* e = operator new(size);
    memset(e, 0xA5, size);
    c = Build(e, false, 1, &names);
    g_count = 0;
    g_watchDelete = e;
    delete static_cast<CGameComponent*>(c);
    g_watchDelete = 0;
    const bool freed = g_count == 17 && Expect(0, EV_SHUTDOWN, e) &&
        Teardown(static_cast<unsigned char*>(e), 1, names) && Expect(16, EV_OP_DELETE, e);

    if (!first || !shared || !noFree || !freed)
    {
        printf("first=%d shared=%d noFree=%d freed=%d\n", first, shared, noFree, freed);
        puts("FABLETLC_NEW_FRONTEND_GAME_COMPONENT_DTOR FAIL");
        return 1;
    }
    puts("FABLETLC_NEW_FRONTEND_GAME_COMPONENT_DTOR PASS");
    return 0;
}