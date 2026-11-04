// Behaviour fixture for CMainGameComponent::InitialiseFonts @ 0x004168DC.
// The component is a raw retail-sized buffer (PCutSceneMainFont at
// +0x1614C/+0x16150); the font manager is a raw 0x3C-byte buffer.
#include <stdio.h>
#include <string.h>
#include "fable_game.h"
#include "fable_gfuninitialise.h"
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


namespace
{
    unsigned char g_fontManagerBytes[0x3C];
    int g_fontMarker;
    CCPPointerInfo g_fontInfo(0, &g_fontMarker);
    const void* g_getFontResult;
}

CFontManager* g_FableFontManager_013B838C =
    reinterpret_cast<CFontManager*>(g_fontManagerBytes);

template<>
CCountedPointer<CFontBank>::CCountedPointer(const CCountedPointer<CFontBank>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}

CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name)
{
    Ev("GetFont", this, &name, 0);
    CCountedPointer<CFontBank> result;
    result.Data = reinterpret_cast<CFontBank*>(&g_fontMarker);
    result.Info = &g_fontInfo;
    ++g_fontInfo.RefCount;
    return result;
}

template<>
CCountedPointer<CFontBank>&
CCountedPointer<CFontBank>::operator=(const CCountedPointer<CFontBank>& other)
{
    Ev("CCountedPointer<CFontBank>::operator=", this, &other, 0);
    Data = other.Data;
    Info = other.Info;
    ++Info->RefCount;
    return *this;
}

struct Access : CMainGameComponent
{
    static void Run(CMainGameComponent* c) { static_cast<Access*>(c)->InitialiseFonts(); }
};

int main()
{
    static unsigned long storage[sizeof(CMainGameComponent) / 4];
    static unsigned char before[sizeof(storage)];
    memset(storage, 0, sizeof(storage));
    memcpy(before, storage, sizeof(storage));
    unsigned char* base = reinterpret_cast<unsigned char*>(storage);
    g_fontInfo.RefCount = 1;
    g_nev = 0; g_seq = 0; g_nstr = 0;

    Access::Run(reinterpret_cast<CMainGameComponent*>(storage));

    int i = 0;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "Init Fonts") == 0 && g_ev[i].b == -1);
    const void* s1 = g_ev[i].self; ++i;
    CHECK(Is(i, "DisplayProgress") && g_ev[i].self == s1 && g_ev[i].b == 1); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == s1); ++i;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "ENG_ARIAL_18") == 0 && g_ev[i].b == -1);
    const void* name = g_ev[i].self; ++i;
    CHECK(Is(i, "GetFont") && g_ev[i].self == g_fontManagerBytes && g_ev[i].a == name); ++i;
    CHECK(Is(i, "CCountedPointer<CFontBank>::operator=") && g_ev[i].self == base + 0x1614C); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == name); ++i;
    CHECK(i == g_nev);

    // The member holds the font; the returned temporary dropped its reference.
    CHECK(*reinterpret_cast<void**>(base + 0x1614C) == &g_fontMarker);
    CHECK(*reinterpret_cast<CCPPointerInfo**>(base + 0x16150) == &g_fontInfo);
    CHECK(g_fontInfo.RefCount == 2);
    memset(base + 0x1614C, 0, 8);
    CHECK(memcmp(storage, before, sizeof(storage)) == 0);
    puts("FABLETLC_MAINGAME_INITIALISEFONTS_004168DC_PASS");
    return 0;
}