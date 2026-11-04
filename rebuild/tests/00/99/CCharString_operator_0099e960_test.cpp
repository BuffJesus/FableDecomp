// Behaviour fixture for CCharString::operator!=(const char*) (0x0099E960).
// Links the compiled source object; the data comparison
// (CCharStringData::operator==, retail 0x004115A0) is a recording stand-in.
#define private public
#include "fable_string.h"
#undef private

#include <stdio.h>
#include <string.h>

static int g_eqCalls;
static const CCharStringData* g_eqThis;
static const char* g_eqOther;
static bool g_eqResult;

bool CCharStringData::operator==(const char* other) const
{
    ++g_eqCalls;
    g_eqThis = this;
    g_eqOther = other;
    return g_eqResult;
}

int main()
{
    int fail = 0;
    unsigned long raw[1] = { 0 };
    const CCharString* s = reinterpret_cast<const CCharString*>(raw);

    // Empty string: equal only to "" (no data comparison).
    if (*s != "") fail |= 1;
    if (!(*s != "x")) fail |= 2;
    if (g_eqCalls != 0) fail |= 4;

    // With data: the negated data comparison, passing the same pointer.
    CCharStringData data;
    raw[0] = reinterpret_cast<unsigned long>(&data);
    const char* text = "Fable";
    g_eqResult = true;
    if (*s != text) fail |= 8;
    if (g_eqCalls != 1 || g_eqThis != &data || g_eqOther != text) fail |= 16;
    g_eqResult = false;
    if (!(*s != text)) fail |= 32;
    if (g_eqCalls != 2) fail |= 64;

    if (fail == 0)
        printf("CCHARSTRING_NOTEQUAL_PASS\n");
    else
        printf("CCHARSTRING_NOTEQUAL_FAIL %d\n", fail);
    return 0;
}