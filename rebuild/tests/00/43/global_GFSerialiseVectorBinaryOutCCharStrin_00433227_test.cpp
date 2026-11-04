// Behaviour of GFSerialiseVectorBinaryOut<CCharString> 0x00433227: writes the
// element count (WriteSLONG), then every string in order through
// WriteNullTerminatedString; one default string (the traits' DefaultVal)
// lives for the whole call.
#include "fable_persist_context.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static char g_log[256];
static char g_buf[64];
static void Log(const char* t) { strcat(g_log, t); }
static const CCharString* g_base;

CCharString::CCharString() { *reinterpret_cast<void**>(this) = 0; Log("str;"); }
CCharString::~CCharString() { Log("~str;"); }
static void* g_out;
void CDataOutputStream::WriteSLONG(long value)
{
    if ((void*)this != g_out) Log("badout;");
    sprintf(g_buf, "count%ld;", value); Log(g_buf);
}
void CDataOutputStream::WriteNullTerminatedString(const CCharString& text)
{
    if ((void*)this != g_out) Log("badout;");
    sprintf(g_buf, "s%d;", (int)(&text - g_base)); Log(g_buf);
}

int main()
{
    static unsigned char streamStore[64], vecStore[sizeof(_STL::vector<CCharString>)];
    static unsigned char strings[3][sizeof(CCharString)];
    g_out = streamStore;
    g_base = reinterpret_cast<CCharString*>(strings);
    _STL::vector<CCharString>& v = *reinterpret_cast<_STL::vector<CCharString>*>(vecStore);
    CDataOutputStream& s = *reinterpret_cast<CDataOutputStream*>(streamStore);
    v._M_start = reinterpret_cast<CCharString*>(strings);
    v._M_finish = v._M_start + 3;
    GFSerialiseVectorBinaryOut(s, v);
    if (strcmp(g_log, "str;count3;s0;s1;s2;~str;") != 0) { printf("three: %s\n", g_log); return 1; }
    g_log[0] = 0;
    v._M_finish = v._M_start;
    GFSerialiseVectorBinaryOut(s, v);
    if (strcmp(g_log, "str;count0;~str;") != 0) { printf("empty: %s\n", g_log); return 1; }
    puts("FABLETLC_GFSERIALISEVECTORBINARYOUT_CCHARSTRING_00433227_PASS");
    return 0;
}
