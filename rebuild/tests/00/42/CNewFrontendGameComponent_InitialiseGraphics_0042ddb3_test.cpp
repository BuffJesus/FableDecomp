#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_graphic_data_bank_init.h"

// Behaviour fixture for CNewFrontendGameComponent::InitialiseGraphics
// @ 0x0042DDB3. The component, system manager and managers are raw
// retail-sized buffers: m_pMenuFont +0x10, m_pFrontEndGraphicBank +0x40,
// CSystemManager::PDisplayManager +0x60.

extern const CCharString g_FableDefaultFrontEndFont_013B875C;
extern CFontManager* g_FableFontManager_013B838C;
extern CGraphicBankManager* g_FableGraphicBankManager_013B837C;

namespace
{
    int g_seq;
    struct Call { int at; const void* self; unsigned long depth; const void* format; int extra; };
    Call g_calls[16];
    int g_nCalls;

    unsigned char g_systemBytes[0x180];
    unsigned char g_displayBytes[0x2C8];
    unsigned char g_fontManagerBytes[0x3C];
    unsigned char g_bankManagerBytes[8];
    unsigned char g_component[0x148];

    CCPPointerInfo g_fontInfo(0, 0);
    CCPPointerInfo g_bankInfo(0, 0);
    int g_fontMarker;
    int g_bankMarker;

    const void* g_charThis[4];
    const char* g_charText[4];
    long g_charLen[4];
    int g_charAt[4];
    int g_nChars;
    int g_charDtorAt;
    const void* g_charDtorThis;

    int g_getFontAt; const void* g_getFontThis; const void* g_getFontName;
    int g_fontAssignAt; const void* g_fontAssignThis;
    int g_initCtorAt; const void* g_initCtorThis;
    int g_pfAt; const void* g_pfThis; int g_pfFormat;
    int g_openAt; const void* g_openThis; const void* g_openName; const void* g_openInit;
    bool g_openA;
    bool g_openB;
    unsigned char g_initSnapshot[0x2C];
    int g_bankAssignAt; const void* g_bankAssignThis;
    int g_sysCalls;

    void Record(const void* self, unsigned long depth, const void* format, int extra)
    {
        Call& c = g_calls[g_nCalls++];
        c.at = ++g_seq;
        c.self = self;
        c.depth = depth;
        c.format = format;
        c.extra = extra;
    }

    unsigned long U32(const unsigned char* p, int o)
    {
        unsigned long v;
        memcpy(&v, p + o, 4);
        return v;
    }
}

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long length) : storage_(0)
{
    int i = g_nChars++;
    if (i < 4)
    {
        g_charThis[i] = this;
        g_charText[i] = text;
        g_charLen[i] = length;
        g_charAt[i] = ++g_seq;
    }
}
CCharString::~CCharString()
{
    if (this != &g_FableDefaultFrontEndFont_013B875C)
    {
        g_charDtorAt = ++g_seq;
        g_charDtorThis = this;
    }
}

const CCharString g_FableDefaultFrontEndFont_013B875C;
CFontManager* g_FableFontManager_013B838C =
    reinterpret_cast<CFontManager*>(g_fontManagerBytes);
CGraphicBankManager* g_FableGraphicBankManager_013B837C =
    reinterpret_cast<CGraphicBankManager*>(g_bankManagerBytes);

// Return-by-value copies in the stand-ins below (retail copy semantics:
// copy both fields, then ++Info->RefCount; the local's dtor drops it again).
template<>
CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer<CFontBank>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

template<>
CCountedPointer<CGraphicDataBank>::CCountedPointer(const CCountedPointer<CGraphicDataBank>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name)
{
    g_getFontAt = ++g_seq;
    g_getFontThis = this;
    g_getFontName = &name;
    CCountedPointer<CFontBank> result;
    result.Data = reinterpret_cast<CFontBank*>(&g_fontMarker);
    result.Info = &g_fontInfo;
    ++g_fontInfo.RefCount;
    return result;
}

template<> template<>
CCountedPointer<const CFontBank>&
CCountedPointer<const CFontBank>::operator=(const CCountedPointer<CFontBank>& other)
{
    g_fontAssignAt = ++g_seq;
    g_fontAssignThis = this;
    Data = other.Data;
    Info = other.Info;
    ++Info->RefCount;
    return *this;
}

template<>
CCountedPointer<CGraphicDataBank>&
CCountedPointer<CGraphicDataBank>::operator=(const CCountedPointer<CGraphicDataBank>& other)
{
    g_bankAssignAt = ++g_seq;
    g_bankAssignThis = this;
    Data = other.Data;
    Info = other.Info;
    ++Info->RefCount;
    return *this;
}

CGraphicDataBankInit::CGraphicDataBankInit()
{
    g_initCtorAt = ++g_seq;
    g_initCtorThis = this;
    GenerateMipmaps = false;
    MaxGraphicWidth = 0;
    MaxGraphicHeight = 0;
    AllowDither = false;
    AllowCompressedTextures = true;
    AllowDXT1ForBooleanAlpha = true;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    ++g_sysCalls;
    return reinterpret_cast<CSystemManager*>(g_systemBytes);
}

bool CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long d, CPixelFormat& f) { Record(this, d, &f, 1); return true; }
bool CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Record(this, d, &f, 2); return true; }
bool CDisplayManager::GetBestCompressedBooleanAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Record(this, d, &f, 3); return true; }
bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long d, CPixelFormat& f) { Record(this, d, &f, 4); return true; }
bool CDisplayManager::GetBestOpaqueTextureFormat(unsigned long d, CPixelFormat& f, bool rt) { Record(this, d, &f, rt ? 51 : 50); return true; }
bool CDisplayManager::GetBestSignedTextureFormat(unsigned long d, CPixelFormat& f) { Record(this, d, &f, 6); return true; }

void CPixelFormat::Initialise(D3DFORMAT format)
{
    g_pfAt = ++g_seq;
    g_pfThis = this;
    g_pfFormat = (int)format;
}

CCountedPointer<CGraphicDataBank> CGraphicBankManager::OpenGraphicBank(
    const CCharString& bankHandle,
    const CGraphicDataBankInit& init,
    bool asSurfaces,
    bool readSymbols)
{
    g_openAt = ++g_seq;
    g_openThis = this;
    g_openName = &bankHandle;
    g_openInit = &init;
    g_openA = asSurfaces;
    g_openB = readSymbols;
    memcpy(g_initSnapshot, &init, sizeof(g_initSnapshot));
    CCountedPointer<CGraphicDataBank> result;
    result.Data = reinterpret_cast<CGraphicDataBank*>(&g_bankMarker);
    result.Info = &g_bankInfo;
    ++g_bankInfo.RefCount;
    return result;
}

int main()
{
    memset(g_component, 0xA5, sizeof(g_component));
    memset(g_component + 0x10, 0, 8);
    memset(g_component + 0x40, 0, 8);
    void* display = g_displayBytes;
    memcpy(g_systemBytes + 0x60, &display, 4);
    g_fontInfo.RefCount = 1;
    g_bankInfo.RefCount = 1;

    CNewFrontendGameComponent* self =
        reinterpret_cast<CNewFrontendGameComponent*>(g_component);
    bool ok = self->InitialiseGraphics();

    // Font: GetFont(DEFAULT_FRONT_END_FONT) on the font manager, assigned
    // into +0x10, then the temporary is released (source + member = 2).
    ok = ok && g_getFontAt == 1 && g_getFontThis == g_fontManagerBytes
        && g_getFontName == &g_FableDefaultFrontEndFont_013B875C;
    ok = ok && g_fontAssignAt == 2 && g_fontAssignThis == g_component + 0x10;
    ok = ok && U32(g_component, 0x10) == (unsigned long)&g_fontMarker
        && U32(g_component, 0x14) == (unsigned long)&g_fontInfo;
    ok = ok && g_fontInfo.RefCount == 2;

    // Init block, then six format queries on the display manager.
    ok = ok && g_initCtorAt == 3;
    const unsigned char* init = (const unsigned char*)g_initCtorThis;
    ok = ok && g_nCalls == 6 && g_sysCalls == 6;
    static const unsigned long depth[6] = { 32, 32, 16, 32, 32, 16 };
    static const int field[6] = { 0x00, 0x04, 0x0C, 0x14, 0x10, 0x18 };
    static const int kind[6] = { 1, 2, 3, 4, 50, 6 };
    for (int i = 0; i < 6 && ok; ++i)
    {
        ok = g_calls[i].at == 4 + i && g_calls[i].self == g_displayBytes
            && g_calls[i].depth == depth[i] && g_calls[i].format == init + field[i]
            && g_calls[i].extra == kind[i];
    }
    ok = ok && g_pfAt == 10 && g_pfThis == init + 0x08 && g_pfFormat == 0x35545844;

    // Bank name, then OpenGraphicBank with the finished init block.
    ok = ok && g_nChars == 1 && g_charAt[0] == 11
        && strcmp(g_charText[0], "GBANK_FRONT_END") == 0 && g_charLen[0] == -1;
    ok = ok && g_openAt == 12 && g_openThis == g_bankManagerBytes
        && g_openName == g_charThis[0] && g_openInit == init && g_openA && g_openB;
    ok = ok && g_initSnapshot[0x1C] == 1 && U32(g_initSnapshot, 0x20) == 0x800
        && U32(g_initSnapshot, 0x24) == 0x800 && g_initSnapshot[0x28] == 1
        && g_initSnapshot[0x29] == 1 && g_initSnapshot[0x2A] == 1;
    ok = ok && g_bankAssignAt == 13 && g_bankAssignThis == g_component + 0x40;
    ok = ok && U32(g_component, 0x40) == (unsigned long)&g_bankMarker
        && U32(g_component, 0x44) == (unsigned long)&g_bankInfo
        && g_bankInfo.RefCount == 2;
    ok = ok && g_charDtorAt == 14 && g_charDtorThis == g_charThis[0];

    // Neighbours untouched.
    for (int i = 0; i < 0x148 && ok; ++i)
    {
        if ((i >= 0x10 && i < 0x18) || (i >= 0x40 && i < 0x48))
            continue;
        if (g_component[i] != 0xA5)
            ok = false;
    }
    if (ok)
        printf("FABLETLC_NEWFRONTEND_INITIALISEGRAPHICS_PASS\n");
    else
        printf("FAIL\n");
    return ok ? 0 : 1;
}