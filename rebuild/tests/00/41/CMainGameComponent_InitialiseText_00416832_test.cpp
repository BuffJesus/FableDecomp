// Behaviour fixture for CMainGameComponent::InitialiseText @ 0x00416832.
// The component is a raw retail-sized buffer (PTextBank at +0x14/+0x18).
// NGameText::CDataBank is a test-local stand-in whose vtable slot 1 plays
// CBankFile::OpenReadOnly(const CCharString&, unsigned long).
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "fable_game.h"
#include "fable_language.h"
#include "fable_progress_display.h"

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


namespace NGameText
{
    class CDataBank
    {
    public:
        CDataBank();
        virtual void Slot0() {}
        virtual bool OpenReadOnly(const CCharString& handle, unsigned long flags);
        unsigned char body[0x210];
    };
}

namespace
{
    unsigned char g_newBuffer[0x214];
    bool g_newReturnsNull;
    NGameText::CDataBank* g_fallbackBank;
}

void* operator new(size_t size)
{
    Ev("operator new", 0, 0, (long)size);
    return g_newReturnsNull ? 0 : g_newBuffer;
}

NGameText::CDataBank::CDataBank()
{
    Ev("CDataBank()", this, 0, 0);
}

bool NGameText::CDataBank::OpenReadOnly(const CCharString& handle, unsigned long flags)
{
    Ev("OpenReadOnly", this, &handle, (long)flags);
    return true;
}

template<>
void CCountedPointer<NGameText::CDataBank>::Reset(NGameText::CDataBank* data)
{
    Ev("Reset", this, data, 0);
    Data = data ? data : g_fallbackBank;
}

CCharString FABLE_FASTCALL NLocalisation::GetTextBankName()
{
    Ev("GetTextBankName", 0, 0, 0);
    return CCharString("TEXT_ENGLISH_MAIN", -1);
}

struct Access : CMainGameComponent
{
    static void Run(CMainGameComponent* c) { static_cast<Access*>(c)->InitialiseText(); }
};

static int RunCase(bool newNull)
{
    static unsigned long storage[sizeof(CMainGameComponent) / 4];
    static unsigned char before[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(before, storage, sizeof(storage));
    unsigned char* base = reinterpret_cast<unsigned char*>(storage);
    CMainGameComponent* c = reinterpret_cast<CMainGameComponent*>(storage);

    // A pre-built bank the Reset stand-in substitutes when new fails, so the
    // null branch can still reach the virtual call.
    static NGameText::CDataBank fallback;
    g_fallbackBank = &fallback;
    g_nev = 0; g_seq = 0; g_nstr = 0;
    g_newReturnsNull = newNull;

    Access::Run(c);

    int i = 0;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "Init Text") == 0 && g_ev[i].b == -1);
    const void* s1 = g_ev[i].self; ++i;
    CHECK(Is(i, "DisplayProgress") && g_ev[i].self == s1 && g_ev[i].b == 1); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == s1); ++i;
    CHECK(Is(i, "operator new") && g_ev[i].b == 0x214); ++i;
    if (!newNull)
    {
        CHECK(Is(i, "CDataBank()") && g_ev[i].self == g_newBuffer); ++i;
        CHECK(Is(i, "Reset") && g_ev[i].self == base + 0x14 && g_ev[i].a == g_newBuffer); ++i;
    }
    else
    {
        CHECK(Is(i, "Reset") && g_ev[i].self == base + 0x14 && g_ev[i].a == 0); ++i;
    }
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "Opening Text Bank") == 0);
    const void* s2 = g_ev[i].self; ++i;
    CHECK(Is(i, "DisplayProgress") && g_ev[i].self == s2 && g_ev[i].b == 1); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == s2); ++i;
    CHECK(Is(i, "GetTextBankName")); ++i;
    CHECK(Is(i, "CCharString(const char*)") && strcmp((const char*)g_ev[i].a, "TEXT_ENGLISH_MAIN") == 0);
    const void* name = g_ev[i].self; ++i;
    const void* bank = newNull ? (const void*)g_fallbackBank : (const void*)g_newBuffer;
    CHECK(Is(i, "OpenReadOnly") && g_ev[i].self == bank && g_ev[i].a == name && g_ev[i].b == 0x100); ++i;
    CHECK(Is(i, "~CCharString") && g_ev[i].self == name); ++i;
    CHECK(i == g_nev);

    // Only PTextBank.Data (+0x14) changed.
    CHECK(*reinterpret_cast<const void* const*>(base + 0x14) == bank);
    memcpy(base + 0x14, before + 0x14, 4);
    CHECK(memcmp(storage, before, sizeof(storage)) == 0);
    return 0;
}

int main()
{
    if (RunCase(false) != 0) return 1;
    if (RunCase(true) != 0) return 1;
    puts("FABLETLC_MAINGAME_INITIALISETEXT_00416832_PASS");
    return 0;
}