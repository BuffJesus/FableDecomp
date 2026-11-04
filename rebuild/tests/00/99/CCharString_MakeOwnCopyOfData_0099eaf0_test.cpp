// Behaviour of CCharString::MakeOwnCopyOfData 0x0099EAF0: an empty string or
// one whose data has a single owner is left alone; shared data is released
// (UnassignString) and replaced by a new one-owner copy built from its text
// with CBasicString(const char*).
#define private public
#include "fable_string.h"
#undef private

// The header declares these private (AAE); the fixture reaches them through the
// public spelling and aliases to the real symbols.
#pragma comment(linker, "/alternatename:?MakeOwnCopyOfData@CCharString@@QAEXXZ=?MakeOwnCopyOfData@CCharString@@AAEXXZ")
#pragma comment(linker, "/alternatename:?UnassignString@CCharString@@AAEXXZ=?UnassignString@CCharString@@QAEXXZ")
#include <stdio.h>
#include <string.h>

// Recording stand-ins (fixture scaffolding).
static const char* g_basicText;
static int g_basics;
template<> CBasicString<char>::CBasicString(const char* text) { ++g_basics; g_basicText = text; PData = (char*)text; StringLength = 7; }
static int g_unassigns;
static CCharStringData* g_released;
void CCharString::UnassignString()
{
    CCharStringData*& s = *reinterpret_cast<CCharStringData**>(this);
    ++g_unassigns;
    g_released = s;
    if (s) { --s->owners; s = 0; }
}
static CCharStringData*& Storage(CCharString& s) { return *reinterpret_cast<CCharStringData**>(&s); }

int main()
{
    unsigned char store[sizeof(CCharString)];
    CCharString& s = *reinterpret_cast<CCharString*>(store);
    Storage(s) = 0;
    s.MakeOwnCopyOfData();
    if (Storage(s) != 0 || g_basics != 0 || g_unassigns != 0) { puts("empty"); return 1; }

    CCharStringData shared;
    shared.Data.PData = (char*)"shared";
    shared.owners = 1;
    Storage(s) = &shared;
    s.MakeOwnCopyOfData();
    if (Storage(s) != &shared || g_basics != 0 || g_unassigns != 0) { puts("single"); return 1; }

    shared.owners = 2;
    s.MakeOwnCopyOfData();
    CCharStringData* own = Storage(s);
    if (own == 0 || own == &shared || g_basics != 1 || g_basicText != shared.Data.PData || own->owners != 1 ||
        g_unassigns != 1 || g_released != &shared || shared.owners != 1) { puts("copy"); return 1; }
    puts("FABLETLC_CCHARSTRING_MAKEOWNCOPY_0099EAF0_PASS");
    return 0;
}