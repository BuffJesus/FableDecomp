// Behaviour of operator+(const char*, const CCharString&) 0x0099F690: a
// non-empty literal becomes a fresh one-owner string (CBasicString(const char*)),
// an empty or null one stays empty; the right string is appended with
// operator+=(const CCharString&); the caller's result shares the joined data
// (the local copy is released) and the instance count rises by one.
#include "fable_string.h"
#include <stdio.h>
#include <string.h>
#include <new>

// Recording stand-ins (fixture scaffolding).
fable_i32 g_CCharStringInstanceCount_013BD800;
static const char* g_basicText;
template<> CBasicString<char>::CBasicString(const char* text) { PData = (char*)text; StringLength = 0; g_basicText = text; }
template<> void CBasicString<char>::SetString(const char*, unsigned long) {}  // unused (length -1 path)
static int g_appends;
static const CCharString* g_appendOther;
static CCharStringData* g_appendData;
static CCharStringData g_rightData;
const CCharString& CCharString::operator+=(const CCharString& other)
{
    ++g_appends;
    g_appendOther = &other;
    g_appendData = storage_;
    if (storage_ == 0)
    {
        storage_ = &g_rightData;
        ++storage_->owners;
    }
    return *this;
}
void CCharString::UnassignString()
{
    if (storage_ != 0)
    {
        --storage_->owners;
        storage_ = 0;
    }
}
struct Peek : CCharString { CCharStringData* Data() const { return *reinterpret_cast<CCharStringData* const*>(this); } };
static CCharStringData* DataOf(const CCharString& s) { return *reinterpret_cast<CCharStringData* const*>(&s); }

int main()
{
    unsigned char rightStore[sizeof(CCharString)];
    memset(rightStore, 0, sizeof(rightStore));
    const CCharString& right = *reinterpret_cast<CCharString*>(rightStore);
    g_rightData.owners = 1;

    unsigned char out[sizeof(CCharString)];
    memset(out, 0xCC, sizeof(out));
    CCharString* joined = new (out) CCharString(operator+("NULLDEF_", right));
    (void)joined;
    CCharStringData* data = DataOf(*reinterpret_cast<CCharString*>(out));
    if (g_basicText == 0 || strcmp(g_basicText, "NULLDEF_") != 0 || g_appends != 1 || g_appendOther != &right) { puts("literal"); return 1; }
    if (data == 0 || data != g_appendData || data->owners != 1 || g_CCharStringInstanceCount_013BD800 != 1) { printf("result %d\n", (int)g_CCharStringInstanceCount_013BD800); return 1; }

    // Empty and null literals start empty: the appended string's data is shared.
    g_basicText = 0;
    unsigned char out2[sizeof(CCharString)];
    new (out2) CCharString(operator+("", right));
    if (g_basicText != 0 || g_appends != 2 || g_appendData != 0 || DataOf(*reinterpret_cast<CCharString*>(out2)) != &g_rightData ||
        g_rightData.owners != 2) { puts("empty"); return 1; }
    unsigned char out3[sizeof(CCharString)];
    new (out3) CCharString(operator+((const char*)0, right));
    if (g_basicText != 0 || g_appends != 3 || g_appendData != 0 || g_rightData.owners != 3 ||
        g_CCharStringInstanceCount_013BD800 != 3) { puts("null"); return 1; }
    puts("FABLETLC_CCHARSTRING_LITERAL_PLUS_STRING_0099F690_PASS");
    return 0;
}