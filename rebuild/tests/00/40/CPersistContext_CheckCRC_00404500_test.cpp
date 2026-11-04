// Behaviour of CPersistContext::CheckCRC(name) 0x00404500: binary load skips
// four bytes of the load stream (in-window, or through the chunk-overflow
// reader when fewer remain; nothing past the 2 GB position limit); binary
// save writes the CRC of the name (seed 0, its text and length; the empty
// string for a null name's data); other modes touch nothing.
#include "fable_persist_context.h"
#include "fable_data_input_stream.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static char g_log[256];
static char g_buf[96];
static void Log(const char* t) { strcat(g_log, t); }

char g_FableEmptyNarrowString_0129AAF4[1];

// A string's data: the CBasicString's buffer and length lead CCharStringData.
struct FakeData { const char* text; unsigned long length; };
static FakeData g_data;
static bool g_nullData;
CCharString::CCharString(const char* text, long length)
{
    if (length != -1) Log("badlen;");
    g_data.text = text; g_data.length = (unsigned long)strlen(text);
    *reinterpret_cast<FakeData**>(this) = g_nullData ? 0 : &g_data;
    Log("str;");
}
CCharString::~CCharString() { Log("~str;"); }

fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 byteCount)
{
    sprintf(g_buf, "crc(%lu,%s,%lu);", (unsigned long)seed, (const char*)data, (unsigned long)byteCount); Log(g_buf);
    return 0xABCD0000UL + byteCount;
}
static void* g_out;
void CDataOutputStream::WriteULONG(unsigned long value)
{
    if ((void*)this != g_out) Log("badout;");
    sprintf(g_buf, "write%lx;", value); Log(g_buf);
}
static void* g_overflowData;
void CDataInputStream::ReadWithSrcChunkOverflow(void* data, long size)
{
    g_overflowData = data;
    sprintf(g_buf, "overflow%ld;", size); Log(g_buf);
}

struct Probe : CPersistContext { void Check(const char* n) { CheckCRC(n); } };
static unsigned char g_ctxStore[sizeof(CPersistContext)];
static unsigned char g_inStore[sizeof(CMemoryDataInputStream)], g_outStore[64];
static fable_u8 g_bytes[16];

static CPersistContext& Context(int mode)
{
    CPersistContext& c = *reinterpret_cast<CPersistContext*>(g_ctxStore);
    memset(g_ctxStore, 0, sizeof(g_ctxStore));
    c.Mode = (CPersistContext::EMode)mode;
    c.PLoadStream = reinterpret_cast<CMemoryDataInputStream*>(g_inStore);
    c.PSaveStream = reinterpret_cast<CDataOutputStream*>(g_outStore);
    g_out = g_outStore;
    g_log[0] = 0;
    return c;
}

static CDataInputStream& Stream(unsigned long pos, long remaining)
{
    CDataInputStream& s = *reinterpret_cast<CDataInputStream*>(g_inStore);
    s.StreamPos = pos; s.StreamSize = 100; s.CurrentSourcePtr = g_bytes; s.SourceChunkStreamPos = 0;
    s.SourceChunkBytesRemaining = remaining;
    return s;
}

int main()
{
    // Load, in window: four bytes skipped.
    CPersistContext& c = Context(CPersistContext::MODE_BINARY_LOAD);
    CDataInputStream& s = Stream(8, 10);
    static_cast<Probe&>(c).Check("Name");
    if (g_log[0] || s.StreamPos != 12 || s.CurrentSourcePtr != g_bytes + 4 || s.SourceChunkBytesRemaining != 6)
    { printf("load: %s\n", g_log); return 1; }
    // Load, three bytes left in the chunk: the overflow reader.
    Context(CPersistContext::MODE_BINARY_LOAD);
    Stream(8, 3);
    static_cast<Probe&>(c).Check("Name");
    if (strcmp(g_log, "overflow4;") != 0 || s.StreamPos != 8 || g_overflowData == 0) { printf("overflow: %s\n", g_log); return 1; }
    // Load at the position limit: nothing.
    Context(CPersistContext::MODE_BINARY_LOAD);
    Stream(0x7FFFFFFD, 10);
    static_cast<Probe&>(c).Check("Name");
    if (g_log[0] || s.StreamPos != 0x7FFFFFFD || s.SourceChunkBytesRemaining != 10) { printf("limit: %s\n", g_log); return 1; }
    // Save: the name's CRC.
    Context(CPersistContext::MODE_BINARY_SAVE);
    static_cast<Probe&>(c).Check("Name");
    if (strcmp(g_log, "str;crc(0,Name,4);writeabcd0004;~str;") != 0) { printf("save: %s\n", g_log); return 1; }
    // Save with null string data: the empty text, length 0.
    g_nullData = true;
    Context(CPersistContext::MODE_BINARY_SAVE);
    static_cast<Probe&>(c).Check("Other");
    if (strcmp(g_log, "str;crc(0,,0);writeabcd0000;~str;") != 0) { printf("null: %s\n", g_log); return 1; }
    g_nullData = false;
    // Other modes: nothing.
    for (int mode = 0; mode <= 4; ++mode)
    {
        if (mode == 2 || mode == 3) continue;
        Context(mode);
        Stream(8, 10);
        static_cast<Probe&>(c).Check("Name");
        if (g_log[0] || s.StreamPos != 8) { printf("mode %d: %s\n", mode, g_log); return 1; }
    }
    puts("FABLETLC_CPERSISTCONTEXT_CHECKCRC_00404500_PASS");
    return 0;
}