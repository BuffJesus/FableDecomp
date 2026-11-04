// Behaviour fixture for CMainGameComponent::InitDisplayEngine @ 0x00417418.
// Checks the CDisplayEngineInit contents handed to the new CDisplayEngine
// (0x100-byte allocation), that the previous engine is deleted after the new
// one is built, the progress message/arguments, the Init call on the new
// engine, call order, and that only PDisplayEngine changes. Covers an old
// engine present/absent.
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_display_engine.h"
#include "fable_player_manager.h"
#include "fable_progress_display.h"
#include "fable_surface.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_support/base_class_stubs.h"

CDisplayManager* g_FableDisplayManager_013B8390;

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->InitDisplayEngine(); }
};

static int seq;
static bool ok;
static unsigned char compStorage[0x161E8 + 16];
static unsigned char* comp = compStorage;
static void Wr(unsigned off, unsigned long v) { memcpy(comp + off, &v, 4); }
static unsigned long Rd(unsigned off) { unsigned long v; memcpy(&v, comp + off, 4); return v; }

static unsigned long displayObject[0x2C8 / 4];
static unsigned char worldStorage[0x198];
static unsigned long defMgrStorage[4];

static int initCtorAt, heightAt, dimsAt, pfAt, getAt, newAt, ctorAt, oldDelAt, strAt, progressAt, strDtorAt, initAt;
static unsigned newSize;
static bool failAlloc;
static unsigned char newEngine[0x100];
static CDisplayEngineInit seen;
static const void* initAddr;

void* operator new(size_t size)
{
    newAt = ++seq;
    newSize = size;
    return failAlloc ? 0 : newEngine;
}
void operator delete(void*) { ok = false; }

CDisplayEngineInit::CDisplayEngineInit()
{
    initCtorAt = ++seq;
    memset(this, 0, sizeof(*this));
    PixelFormat.FormatIndex = -1;
    initAddr = this;
}
unsigned long CSurface::GetHeight() const
{
    heightAt = ++seq;
    ok = ok && static_cast<const void*>(this) == reinterpret_cast<unsigned char*>(displayObject) + 0x2C;
    return 768;
}
C2DExtentsI CSurface::GetDimensions() const
{
    dimsAt = ++seq;
    ok = ok && static_cast<const void*>(this) == reinterpret_cast<unsigned char*>(displayObject) + 0x2C;
    return C2DExtentsI(1024, 999);
}
CPixelFormat CSurface::GetPixelFormat() const
{
    pfAt = ++seq;
    ok = ok && static_cast<const void*>(this) == reinterpret_cast<unsigned char*>(displayObject) + 0x2C;
    CPixelFormat f;
    f.FormatIndex = 22;
    return f;
}
CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    getAt = ++seq;
    return *reinterpret_cast<CGameDefinitionManager*>(defMgrStorage);
}
CDisplayEngine::CDisplayEngine(const CDisplayEngineInit& init)
{
    ctorAt = ++seq;
    ok = ok && static_cast<void*>(this) == newEngine && &init == initAddr;
    seen = init;
}

// The stand-in constructor above needs CDisplayEngine's other real members.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDisplayEngine::~CDisplayEngine() { ok = false; }
bool CDisplayEngine::InitEngine() { ok = false; return false; }
void CDisplayEngine::UninitEngine() { ok = false; }

static unsigned long oldEngine[4];
static void* oldVtable[4];
static unsigned long oldDelFlags;
static void* FABLE_FASTCALL OldDel(void* self, void*, unsigned flags)
{
    oldDelAt = ++seq;
    oldDelFlags = flags;
    ok = ok && self == oldEngine && Rd(0x28) == (failAlloc ? 0 : (unsigned long)newEngine);
    return self;
}
static void FABLE_FASTCALL Bad(void*, void*) { ok = false; }

static const char* strText;
static long strLen;
static void* strThis;
CCharString::CCharString(const char* text, long length)
{
    strAt = ++seq;
    strText = text;
    strLen = length;
    strThis = this;
}
CCharString::~CCharString()
{
    strDtorAt = ++seq;
    ok = ok && static_cast<void*>(this) == strThis;
}
static float progFraction;
static bool progForce, progDebug;
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    progressAt = ++seq;
    ok = ok && static_cast<const void*>(&text) == strThis;
    progFraction = fraction;
    progForce = forceRender;
    progDebug = isDebug;
}
static bool initCalled;
void CDisplayEngine::Init()
{
    initAt = ++seq;
    ok = ok && static_cast<void*>(this) == newEngine;
}

static bool Run(bool haveOld, bool fail)
{
    memset(compStorage, 0xA5, sizeof(compStorage));
    memset(worldStorage, 0, sizeof(worldStorage));
    unsigned long mesh = 0x3E5B0000;
    memcpy(worldStorage + 0x3C, &mesh, 4);
    Wr(0x1C, 0x0000D1C0);                           // PPlayerManager
    Wr(0x24, (unsigned long)worldStorage);          // PWorld
    Wr(0x28, haveOld ? (unsigned long)oldEngine : 0);
    Wr(0x161C8, 0x6B6B61C8);                        // PMainGraphicBank.Data
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));
    oldVtable[0] = (void*)&OldDel;
    oldVtable[1] = oldVtable[2] = oldVtable[3] = (void*)&Bad;
    oldEngine[0] = (unsigned long)oldVtable;
    g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayObject);

    seq = 0;
    initCtorAt = heightAt = dimsAt = pfAt = getAt = newAt = ctorAt = oldDelAt = strAt = progressAt = strDtorAt = initAt = 0;
    newSize = 0;
    failAlloc = fail;
    memset(&seen, 0x77, sizeof(seen));
    oldDelFlags = 0;
    strText = 0; strLen = 0; strThis = 0;
    progFraction = 0.0f; progForce = progDebug = true;
    ok = true;

    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));

    if (!(initCtorAt == 1 && heightAt == 2 && dimsAt == 3 && pfAt == 4 && getAt == 5 && newAt == 6)) ok = false;
    int n = 6;
    if (!fail) { if (ctorAt != ++n) ok = false; } else if (ctorAt) ok = false;
    if (haveOld) { if (oldDelAt != ++n || oldDelFlags != 1) ok = false; } else if (oldDelAt) ok = false;
    if (strAt != ++n || progressAt != ++n || strDtorAt != ++n) ok = false;
    if (!fail) { if (initAt != ++n) ok = false; }
    if (newSize != 0x100) ok = false;
    if (!fail)
    {
        if (seen.Component != reinterpret_cast<CMainGameComponent*>(comp) ||
            seen.DefinitionManager != reinterpret_cast<CGameDefinitionManager*>(defMgrStorage) ||
            seen.PlayerManager != reinterpret_cast<CPlayerManager*>(0xD1C0) ||
            seen.World != reinterpret_cast<CWorld*>(worldStorage) ||
            seen.MainGraphicBank != reinterpret_cast<CGraphicDataBank*>(0x6B6B61C8) ||
            seen.Width != 1024 || seen.Height != 768 ||
            seen.PixelFormat.FormatIndex != 22 ||
            seen.MeshBank != reinterpret_cast<CMeshDataBank*>(0x3E5B0000))
            ok = false;
    }
    if (strText == 0 || strcmp(strText, "Init Display Engine: Calling Init") != 0 || strLen != -1) ok = false;
    if (progFraction != -1.0f || progForce || progDebug) ok = false;

    unsigned long want = fail ? 0 : (unsigned long)newEngine;
    memcpy(before + 0x28, &want, 4);
    if (memcmp(before, compStorage, sizeof(compStorage)) != 0) ok = false;
    if (!ok) printf("case old=%d fail=%d failed\n", haveOld, fail);
    return ok;
}

int main()
{
    bool pass = true;
    pass = Run(true, false) && pass;
    pass = Run(false, false) && pass;
    if (pass)
        printf("FABLETLC_MAINGAME_INITDISPLAYENGINE PASS\n");
    else
        printf("FABLETLC_MAINGAME_INITDISPLAYENGINE FAIL\n");
    return pass ? 0 : 1;
}