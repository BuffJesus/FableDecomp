// Behaviour fixture for CMainGameComponent::InitGraphics @ 0x00416C8A.
// Raw retail-sized component (PMainGraphicBank at +0x161C8/+0x161CC); the
// display and graphic-bank managers are raw buffers. Runs all four
// combinations of the compressed-texture condition and checks the finished
// CGraphicDataBankInit handed to OpenGraphicBank.
#include <stdio.h>
#include <string.h>
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_graphic_appearance.h"
#include "fable_graphic_data_bank_init.h"
#include "fable_progress_display.h"
#include "fable_system_manager.h"

// ---- shared recording stand-ins (CCharString + NProgressDisplay) ----
namespace
{
    int g_seq;
    struct Event { int at; const char* what; const void* self; const void* a; long b; };
    Event g_ev[128];
    int g_nev;
    void Ev(const char* what, const void* self, const void* a, long b)
    {
        if (g_nev < 128)
        {
            Event& e = g_ev[g_nev++];
            e.at = ++g_seq; e.what = what; e.self = self; e.a = a; e.b = b;
        }
    }
    // Live narrow strings: address -> literal text.
    const void* g_strSelf[64];
    const char* g_strText[64];
    int g_nstr;
    void StrBind(const void* self, const char* text)
    {
        for (int i = 0; i < g_nstr; ++i)
            if (g_strSelf[i] == self) { g_strText[i] = text; return; }
        g_strSelf[g_nstr] = self; g_strText[g_nstr] = text; ++g_nstr;
    }
    const char* StrText(const void* self)
    {
        for (int i = 0; i < g_nstr; ++i)
            if (g_strSelf[i] == self) return g_strText[i];
        return "<unbound>";
    }
    int Fail(int line)
    {
        printf("FAIL line %d\n", line);
        return 1;
    }
    bool Is(int i, const char* what) { return i < g_nev && strcmp(g_ev[i].what, what) == 0; }
}
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)

CCharString::CCharString(const char* text, long length)
{
    StrBind(this, text);
    Ev("CCharString(const char*)", this, text, length);
}
CCharString::CCharString(const CCharString& other)
{
    StrBind(this, StrText(&other));
    Ev("CCharString(copy)", this, &other, 0);
}
CCharString::~CCharString()
{
    Ev("~CCharString", this, StrText(this), 0);
}

float g_lastFraction;
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(
    const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    g_lastFraction = fraction;
    Ev("DisplayProgress", &text, StrText(&text),
       (fraction == -1.0f ? 1 : 0) | (forceRender ? 2 : 0) | (isDebug ? 4 : 0));
}


bool g_FableUseCompressedTextures_01375453;
bool g_FableDitheringOn_01375452;
unsigned long g_FableTextureBitDepth_013B7D68;
unsigned long g_FableMaxTextureWidth_013B7D64;
unsigned long g_FableMaxTextureHeight_013B7D60;

namespace
{
    unsigned char g_displayBytes[0x2C8];
    unsigned char g_bankManagerBytes[8];
    int g_bankMarker;
    CCPPointerInfo g_bankInfo(0, &g_bankMarker);
    bool g_supportsCompressed;
    const unsigned char* g_init;
    unsigned char g_initSnapshot[0x2C];
}

CDisplayManager* g_FableDisplayManager_013B8390 =
    reinterpret_cast<CDisplayManager*>(g_displayBytes);
CGraphicBankManager* g_FableGraphicBankManager_013B837C =
    reinterpret_cast<CGraphicBankManager*>(g_bankManagerBytes);

CGraphicDataBankInit::CGraphicDataBankInit()
{
    Ev("CGraphicDataBankInit()", this, 0, 0);
    g_init = reinterpret_cast<const unsigned char*>(this);
    memset(this, 0xEE, sizeof(*this));
}

// Each format query records the queried field as an offset into the init block.
static long FieldOf(const CPixelFormat& f)
{
    return (long)(reinterpret_cast<const unsigned char*>(&f) - g_init);
}
bool CDisplayManager::IsSupportingCompressedTextures() { Ev("IsSupportingCompressedTextures", this, 0, 0); return g_supportsCompressed; }
bool CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long d, CPixelFormat& f) { Ev("CompressedOpaque", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Ev("CompressedAlpha", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestCompressedBooleanAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Ev("CompressedBooleanAlpha", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Ev("Alpha", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestBooleanAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Ev("BooleanAlpha", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestOpaqueTextureFormat(unsigned long d, CPixelFormat& f, bool rt) { Ev(rt ? "Opaque(rt)" : "Opaque", this, (void*)d, FieldOf(f)); return true; }
bool CDisplayManager::GetBestSignedTextureFormat(unsigned long d, CPixelFormat& f) { Ev("Signed", this, (void*)d, FieldOf(f)); return true; }

void CPixelFormat::Initialise(D3DFORMAT format)
{
    Ev("CPixelFormat::Initialise", this, (void*)format, FieldOf(*this));
}

template<>
CCountedPointer<CGraphicDataBank>::CCountedPointer(const CCountedPointer<CGraphicDataBank>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

CCountedPointer<CGraphicDataBank> CGraphicBankManager::OpenGraphicBank(
    const CCharString& bankHandle, const CGraphicDataBankInit& init, bool asSurfaces, bool readSymbols)
{
    Ev("OpenGraphicBank", this, &bankHandle, (asSurfaces ? 1 : 0) | (readSymbols ? 2 : 0));
    if (reinterpret_cast<const unsigned char*>(&init) != g_init)
        Ev("OpenGraphicBank:wrong init", this, &init, 0);
    memcpy(g_initSnapshot, &init, sizeof(g_initSnapshot));
    CCountedPointer<CGraphicDataBank> result;
    result.Data = reinterpret_cast<CGraphicDataBank*>(&g_bankMarker);
    result.Info = &g_bankInfo;
    ++g_bankInfo.RefCount;
    return result;
}

template<>
CCountedPointer<CGraphicDataBank>&
CCountedPointer<CGraphicDataBank>::operator=(const CCountedPointer<CGraphicDataBank>& other)
{
    Ev("CCountedPointer<CGraphicDataBank>::operator=", this, &other, 0);
    Data = other.Data;
    Info = other.Info;
    ++Info->RefCount;
    return *this;
}

void FABLE_FASTCALL NGraphicAppearance::CTCGraphicAppearance::SetGraphicBank(CGraphicDataBank& bank)
{
    Ev("SetGraphicBank", &bank, 0, 0);
}

struct Access : CMainGameComponent
{
    static bool Run(CMainGameComponent* c) { return static_cast<Access*>(c)->InitGraphics(); }
};

static int Progress(int& i, const char* text)
{
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, text) == 0 && g_ev[i].b == -1);
    const void* s = g_ev[i].self; ++i;
    CHECK(Is(i, "DisplayProgress") && g_ev[i].self == s && g_ev[i].b == 1); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == s); ++i;
    return 0;
}

static int Query(int& i, const char* what, long field)
{
    CHECK(Is(i, what) && g_ev[i].self == g_displayBytes && g_ev[i].a == (void*)32 && g_ev[i].b == field);
    ++i;
    return 0;
}

static int RunCase(bool supports, bool useCompressed, bool dither)
{
    static unsigned long storage[sizeof(CMainGameComponent) / 4];
    static unsigned char before[sizeof(storage)];
    memset(storage, 0, sizeof(storage));
    memcpy(before, storage, sizeof(storage));
    unsigned char* base = reinterpret_cast<unsigned char*>(storage);
    g_bankInfo.RefCount = 1;
    g_nev = 0; g_seq = 0; g_nstr = 0;
    g_supportsCompressed = supports;
    g_FableUseCompressedTextures_01375453 = useCompressed;
    g_FableDitheringOn_01375452 = dither;
    g_FableTextureBitDepth_013B7D68 = 32;
    g_FableMaxTextureWidth_013B7D64 = 1024;
    g_FableMaxTextureHeight_013B7D60 = 512;

    CHECK(Access::Run(reinterpret_cast<CMainGameComponent*>(storage)) == true);

    int i = 0;
    if (Progress(i, "Init Graphics")) return 1;
    CHECK(Is(i, "CGraphicDataBankInit()")); ++i;
    CHECK(Is(i, "IsSupportingCompressedTextures") && g_ev[i].self == g_displayBytes); ++i;
    const bool compressed = supports && useCompressed;
    if (compressed)
    {
        if (Query(i, "CompressedOpaque", 0x00)) return 1;
        if (Query(i, "CompressedAlpha", 0x04)) return 1;
        if (Query(i, "CompressedBooleanAlpha", 0x0C)) return 1;
        CHECK(Is(i, "CPixelFormat::Initialise") && g_ev[i].a == (void*)D3DFMT_DXT5 && g_ev[i].b == 0x08); ++i;
    }
    else
    {
        if (Query(i, "Opaque", 0x00)) return 1;
        if (Query(i, "Alpha", 0x04)) return 1;
        if (Query(i, "BooleanAlpha", 0x0C)) return 1;
    }
    if (Query(i, "Alpha", 0x14)) return 1;
    if (Query(i, "Opaque", 0x10)) return 1;
    if (Query(i, "Signed", 0x18)) return 1;
    if (Progress(i, "Opening Main Graphic Bank")) return 1;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "GBANK_MAIN") == 0);
    const void* bankName = g_ev[i].self; ++i;
    CHECK(Is(i, "OpenGraphicBank") && g_ev[i].self == g_bankManagerBytes && g_ev[i].a == bankName && g_ev[i].b == 3); ++i;
    CHECK(Is(i, "CCountedPointer<CGraphicDataBank>::operator=") && g_ev[i].self == base + 0x161C8); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == bankName); ++i;
    if (Progress(i, "Setting Main Graphic Bank")) return 1;
    CHECK(Is(i, "SetGraphicBank") && g_ev[i].self == &g_bankMarker); ++i;
    CHECK(i == g_nev);

    // The finished init block: the ctor's 0xEE fill survives wherever the
    // function itself does not store (the format stand-ins write nothing).
    const CGraphicDataBankInit& snap = *reinterpret_cast<const CGraphicDataBankInit*>(g_initSnapshot);
    CHECK(snap.GenerateMipmaps == true);
    CHECK(snap.MaxGraphicWidth == 1024 && snap.MaxGraphicHeight == 512);
    CHECK(snap.AllowDither == dither);
    for (int b = 0; b < 0x1C; ++b) CHECK(g_initSnapshot[b] == 0xEE);
    for (int b = 0x1D; b < 0x20; ++b) CHECK(g_initSnapshot[b] == 0xEE);
    for (int b = 0x29; b < 0x2C; ++b) CHECK(g_initSnapshot[b] == 0xEE);

    CHECK(*reinterpret_cast<void**>(base + 0x161C8) == &g_bankMarker);
    CHECK(*reinterpret_cast<CCPPointerInfo**>(base + 0x161CC) == &g_bankInfo);
    CHECK(g_bankInfo.RefCount == 2);
    memset(base + 0x161C8, 0, 8);
    CHECK(memcmp(storage, before, sizeof(storage)) == 0);
    return 0;
}

int main()
{
    if (RunCase(true, true, true)) return 1;
    if (RunCase(true, false, false)) return 1;
    if (RunCase(false, true, true)) return 1;
    if (RunCase(false, false, false)) return 1;
    puts("FABLETLC_MAINGAME_INITGRAPHICS_00416C8A_PASS");
    return 0;
}