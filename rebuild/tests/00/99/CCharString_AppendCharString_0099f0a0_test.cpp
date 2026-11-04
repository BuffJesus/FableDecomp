// Behaviour of CCharString::operator+=(const CCharString&) 0x0099F0A0: a
// non-empty string first takes its own copy of the data, then appends the
// other's text with its length ("" for an empty other); an empty string
// shares the other's data (one more owner) unless the other is itself or
// empty. Returns *this.
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

// Recording stand-ins (fixture scaffolding).
char g_FableEmptyNarrowString_0129AAF4[1];
static int g_copies;
static const void* g_copyThis;
static int g_appends;
static const void* g_appendThis;
static const char* g_appendText;
static unsigned long g_appendLength;
void CCharString::MakeOwnCopyOfData() { ++g_copies; g_copyThis = this; }
template<> void CBasicString<char>::Append(const char* text, unsigned long length)
{
    ++g_appends;
    g_appendThis = this;
    g_appendText = text;
    g_appendLength = length;
}
static CCharStringData*& Storage(CCharString& s) { return *reinterpret_cast<CCharStringData**>(&s); }

int main()
{
    unsigned char aStore[sizeof(CCharString)], bStore[sizeof(CCharString)];
    CCharString& a = *reinterpret_cast<CCharString*>(aStore);
    CCharString& b = *reinterpret_cast<CCharString*>(bStore);
    CCharStringData da, db;
    da.Data.PData = (char*)"left"; da.owners = 1;
    db.Data.PData = (char*)"right"; db.owners = 1;

    Storage(a) = &da;
    Storage(b) = &db;
    if (&(a += b) != &a || g_copies != 1 || g_copyThis != &a || g_appends != 1 || g_appendThis != &da.Data ||
        g_appendText != db.Data.PData || g_appendLength != 5) { puts("append"); return 1; }
    Storage(b) = 0;
    a += b;
    if (g_appends != 2 || g_appendText != g_FableEmptyNarrowString_0129AAF4 || g_appendLength != 0) { puts("append empty"); return 1; }

    Storage(a) = 0;
    Storage(b) = &db;
    if (&(a += b) != &a || Storage(a) != &db || db.owners != 2 || g_copies != 2 || g_appends != 2) { puts("share"); return 1; }
    Storage(a) = 0;
    Storage(b) = 0;
    a += b;
    if (Storage(a) != 0) { puts("empty other"); return 1; }
    puts("FABLETLC_CCHARSTRING_APPEND_STRING_0099F0A0_PASS");
    return 0;
}
