#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "test_support/base_class_stubs.h"

// Fixture for ~CMainGameComponent 0x00418B7A and its scalar deleting
// destructor 0x004197B3. Every observable step is logged as (kind, pointer,
// argument) and checked in order.
namespace
{
    enum Kind
    {
        EV_SHUTDOWN = 1,
        EV_WIDE,
        EV_CHAR,
        EV_DELETE_DATA,
        EV_INIT_BASE,
        EV_DISK_CLOSE,
        EV_NETWORK_CLIENT,
        EV_POLY_DELETE,
        EV_BASE,
        EV_OP_DELETE
    };

    struct Event
    {
        int kind;
        const void* ptr;
        unsigned long arg;
    };

    Event g_log[64];
    int g_count;
    const void* g_watchDelete;

    void Log(int kind, const void* ptr, unsigned long arg = 0)
    {
        if (g_count < 64)
        {
            g_log[g_count].kind = kind;
            g_log[g_count].ptr = ptr;
            g_log[g_count].arg = arg;
        }
        ++g_count;
    }

    bool Expect(int index, int kind, const void* ptr, unsigned long arg = 0)
    {
        return index < g_count && index < 64 &&
            g_log[index].kind == kind && g_log[index].ptr == ptr && g_log[index].arg == arg;
    }

    void FABLE_FASTCALL DeleteRecorder(void* data)
    {
        Log(EV_DELETE_DATA, data);
    }

    // The scoped_ptr pointees are faked as {vptr} objects whose slot 0 (the
    // scalar deleting destructor retail calls with flag 1) records itself.
    struct PolyRecorder
    {
        void* Delete(unsigned int flags)
        {
            Log(EV_POLY_DELETE, this, flags);
            return this;
        }
    };

    void* g_polyVtable[4];
    struct FakePoly
    {
        void** vptr;
        int payload;
    };
    FakePoly g_poly[4];   // PPlayerManager, PPlayerInterface, PWorld, PDisplayEngine

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
}

void operator delete(void* block)
{
    if (block != 0 && block == g_watchDelete)
        Log(EV_OP_DELETE, block);
    free(block);
}

// Recorders / link stand-ins for the real callees.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { Log(EV_BASE, this); }
CBaseClass::~CBaseClass() {}
CInitBaseClass::~CInitBaseClass() { Log(EV_INIT_BASE, this); }
void CInitBaseClass::Validate() const {}
void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
CGameComponent::CGameComponent(CGame& game) { Game = &game; Quit = false; Running = false; }
void CGameComponent::Init() {}
void CGameComponent::SetQuit() {}
void CMainGameComponent::Init() {}
bool CMainGameComponent::Run(CGameComponent**) { return false; }
void CMainGameComponent::ChangeTextureColourDepth(long) {}
void CMainGameComponent::SetQuit() {}
void CMainGameComponent::OnPreDeviceReset() {}
bool CMainGameComponent::OnPostDeviceReset() { return true; }
void CMainGameComponent::Update() {}
void CMainGameComponent::GetInputs() {}
void CMainGameComponent::Render() {}
void CMainGameComponent::PostInit() {}
bool CMainGameComponent::IsEditorActive() const { return false; }
void* g_mainVptr[2];
void CMainGameComponent::Shutdown()
{
    // The destructor has already stored CMainGameComponent's two vptrs.
    memcpy(g_mainVptr, this, sizeof(g_mainVptr));
    Log(EV_SHUTDOWN, this);
}
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString&) : storage_(0) {}
CWideString::~CWideString() { Log(EV_WIDE, this); }
CCharString::~CCharString() { Log(EV_CHAR, this); }

void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { Log(EV_DISK_CLOSE, this); }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
// Link stand-in only (never called): complete the opaque type locally.
class CFileInformation { public: int unused; };
CFileInformation CAFile::GetFileInformation() { CFileInformation info; info.unused = 0; return info; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation info; info.unused = 0; return info; }

namespace
{
    int g_data[6];

    // Counted pointer slots (Data, Info pairs), in declaration order:
    // PTextBank, PLUA, PDebugFont, PCutSceneMainFont, PMainGraphicBank.
    const unsigned long kCounted[5] = { 0x14, 0x2C, 0x16144, 0x1614C, 0x161C8 };
    // scoped_ptr slots: PPlayerManager, PPlayerInterface, PWorld, PDisplayEngine.
    const unsigned long kScoped[4] = { 0x1C, 0x20, 0x24, 0x28 };

    const unsigned long kEventPackageFile = 0x16154;
    const unsigned long kFrameRateSmoother = 0x16178;
    const unsigned long kInitStructure = 0x161D0;
    const unsigned long kClient = 0x13AB8;

    void Build(unsigned char* p, bool quit, unsigned long refCount, bool withPolys)
    {
        memset(p, 0xA5, sizeof(CMainGameComponent));
        // vptrs as the real ctor leaves them are irrelevant: the destructor
        // stores its own. Quit is the only flag it reads.
        p[8] = quit ? 1 : 0;
        for (int i = 0; i < 5; ++i)
        {
            CCPPointerInfo* info = new CCPPointerInfo(DeleteRecorder, &g_data[i]);
            info->RefCount = refCount;
            PutDword(p, kCounted[i], &g_data[i]);
            PutDword(p, kCounted[i] + 4, info);
        }
        for (int i = 0; i < 4; ++i)
            PutDword(p, kScoped[i], withPolys ? &g_poly[i] : 0);
        // FrameRateSmoother.Times: an empty STLport vector (+4..+0xC).
        PutDword(p, kFrameRateSmoother + 0x04, 0);
        PutDword(p, kFrameRateSmoother + 0x08, 0);
        PutDword(p, kFrameRateSmoother + 0x0C, 0);
        PutDword(p, 0x10, (void*)0x12345678);     // PSampleBank: raw, untouched
    }

    // Member teardown after the optional Shutdown, from log index `i`.
    bool Teardown(const unsigned char* p, int i, bool withPolys)
    {
        bool ok =
            Expect(i++, EV_WIDE, p + kInitStructure + 0x0C) &&    // InitStructure.SaveGameName
            Expect(i++, EV_CHAR, p + kInitStructure + 0x08) &&    // .InitialQuestName
            Expect(i++, EV_WIDE, p + kInitStructure + 0x04) &&    // .InitialWorldHolySiteName
            Expect(i++, EV_WIDE, p + kInitStructure + 0x00) &&    // .InitialWorldName
            Expect(i++, EV_DELETE_DATA, &g_data[4]) &&            // PMainGraphicBank
            Expect(i++, EV_INIT_BASE, p + kFrameRateSmoother) &&  // FrameRateSmoother
            Expect(i++, EV_DISK_CLOSE, p + kEventPackageFile) &&  // EventPackageFile
            Expect(i++, EV_WIDE, p + kEventPackageFile + 0x18) && //   .Pathname
            Expect(i++, EV_BASE, p + kEventPackageFile) &&        //   CBaseClassNonCopyable teardown
            Expect(i++, EV_DELETE_DATA, &g_data[3]) &&            // PCutSceneMainFont
            Expect(i++, EV_DELETE_DATA, &g_data[2]) &&            // PDebugFont
            Expect(i++, EV_INIT_BASE, p + kClient) &&             // Client (implicit dtor -> ~CInitBaseClass)
            Expect(i++, EV_DELETE_DATA, &g_data[1]);              // PLUA
        if (withPolys)
        {
            ok = ok &&
                Expect(i++, EV_POLY_DELETE, &g_poly[3], 1) &&     // PDisplayEngine
                Expect(i++, EV_POLY_DELETE, &g_poly[2], 1) &&     // PWorld
                Expect(i++, EV_POLY_DELETE, &g_poly[1], 1) &&     // PPlayerInterface
                Expect(i++, EV_POLY_DELETE, &g_poly[0], 1);       // PPlayerManager
        }
        return ok &&
            Expect(i++, EV_DELETE_DATA, &g_data[0]) &&            // PTextBank
            Expect(i++, EV_BASE, p) && i == g_count;
    }

    bool Cleared(const unsigned char* p)
    {
        for (int i = 0; i < 5; ++i)
            if (Dword(p, kCounted[i]) != 0 || Dword(p, kCounted[i] + 4) != 0)
                return false;
        return Dword(p, 0x10) == 0x12345678;
    }
}

int main()
{
    union
    {
        void* (PolyRecorder::*method)(unsigned int);
        void* address;
    } slot0;
    slot0.method = &PolyRecorder::Delete;
    g_polyVtable[0] = slot0.address;
    for (int i = 0; i < 4; ++i)
        g_poly[i].vptr = g_polyVtable;

    const unsigned long size = sizeof(CMainGameComponent);

    // 1. Not quit, last references, all scoped_ptrs set: Shutdown first, then
    //    every member in reverse declaration order, then ~CGameComponent.
    unsigned char* a = static_cast<unsigned char*>(malloc(size));
    Build(a, false, 1, true);
    g_count = 0;
    reinterpret_cast<CMainGameComponent*>(a)->CMainGameComponent::~CMainGameComponent();
    const bool first = g_count == 20 && Expect(0, EV_SHUTDOWN, a) && Teardown(a, 1, true) &&
        Cleared(a) && Dword(a, 4) != 0xA5A5A5A5;
    free(a);

    // 2. Already quit, shared references, null scoped_ptrs: no Shutdown, no
    //    deletes (counts drop to 1), no virtual deletes through null.
    unsigned char* b = static_cast<unsigned char*>(malloc(size));
    Build(b, true, 2, false);
    CCPPointerInfo* infos[5];
    for (int i = 0; i < 5; ++i)
        infos[i] = reinterpret_cast<CCPPointerInfo*>(Dword(b, kCounted[i] + 4));
    g_count = 0;
    reinterpret_cast<CMainGameComponent*>(b)->CMainGameComponent::~CMainGameComponent();
    bool shared = Cleared(b) && g_count == 10 &&
        Expect(0, EV_WIDE, b + kInitStructure + 0x0C) &&
        Expect(1, EV_CHAR, b + kInitStructure + 0x08) &&
        Expect(2, EV_WIDE, b + kInitStructure + 0x04) &&
        Expect(3, EV_WIDE, b + kInitStructure + 0x00) &&
        Expect(4, EV_INIT_BASE, b + kFrameRateSmoother) &&
        Expect(5, EV_DISK_CLOSE, b + kEventPackageFile) &&
        Expect(6, EV_WIDE, b + kEventPackageFile + 0x18) &&
        Expect(7, EV_BASE, b + kEventPackageFile) &&
        Expect(8, EV_INIT_BASE, b + kClient) &&
        Expect(9, EV_BASE, b);
    for (int i = 0; i < 5; ++i)
    {
        shared = shared && infos[i]->RefCount == 1;
        delete infos[i];
    }
    free(b);

    // 3. Scalar deleting destructor, flag 0: destroys but does not free.
    unsigned char* d = static_cast<unsigned char*>(malloc(size));
    Build(d, false, 1, true);
    memcpy(d, g_mainVptr, sizeof(g_mainVptr));
    g_count = 0;
    g_watchDelete = d;
    reinterpret_cast<CGameComponent*>(d)->~CGameComponent();
    g_watchDelete = 0;
    const bool noFree = g_count == 20 && Expect(0, EV_SHUTDOWN, d) && Teardown(d, 1, true);
    free(d);

    // 4. Scalar deleting destructor, flag 1: destroys, then frees this.
    unsigned char* e = static_cast<unsigned char*>(operator new(size));
    Build(e, false, 1, true);
    memcpy(e, g_mainVptr, sizeof(g_mainVptr));
    g_count = 0;
    g_watchDelete = e;
    delete reinterpret_cast<CGameComponent*>(e);
    g_watchDelete = 0;
    const bool freed = g_count == 21 && Expect(0, EV_SHUTDOWN, e) &&
        Expect(20, EV_OP_DELETE, e);

    if (!first || !shared || !noFree || !freed)
    {
        printf("first=%d shared=%d noFree=%d freed=%d count=%d\n", first, shared, noFree, freed, g_count);
        for (int i = 0; i < g_count && i < 64; ++i)
            printf("  %d kind=%d ptr=%p arg=%lu\n", i, g_log[i].kind, g_log[i].ptr, g_log[i].arg);
        puts("FABLETLC_MAIN_GAME_COMPONENT_DTOR FAIL");
        return 1;
    }
    puts("FABLETLC_MAIN_GAME_COMPONENT_DTOR PASS");
    return 0;
}