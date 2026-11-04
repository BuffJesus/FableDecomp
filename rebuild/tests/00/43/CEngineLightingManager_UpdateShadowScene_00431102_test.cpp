// Behaviour of CPersistContext's binary-only member transfer <long>
// 0x00431102: an unnamed CRC check, then (zeroed traits) TransferBinaryIn
// from the load stream when loading, WriteSLONG of the value to the save
// stream when saving, and nothing in the other modes.
#include "fable_persist_context.h"
#include <stdio.h>
#include <string.h>

static char g_log[512];
static char g_buf[64];
static void Log(const char* t) { strcat(g_log, t); }
static const void* g_base;
static void LogP(const char* t, const void* p) { sprintf(g_buf, "%s%d;", t, (int)((const char*)p - (const char*)g_base)); Log(g_buf); }

void CPersistContext::CheckCRC(const char* name) { Log(name && name[0] == 0 ? "crc;" : "crc?;"); }
static void* g_in; static void* g_out;
template <> void FABLE_FASTCALL GFSerialiseVectorBinaryIn<CCharString>(CMemoryDataInputStream& stream, _STL::vector<CCharString>& value)
{
    if ((void*)&stream != g_in) Log("badin;");
    LogP("vin", &value);
}
template <> void FABLE_FASTCALL GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream& stream, _STL::vector<CCharString>& value)
{
    if ((void*)&stream != g_out) Log("badout;");
    LogP("vout", &value);
}
void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream, long& value)
{
    if ((void*)&stream != g_in) Log("badin;");
    if (DefaultVal != 0) Log("traits?;");
    LogP("lin", &value);
    value = 77;
}
void CDataOutputStream::WriteSLONG(long value)
{
    if ((void*)this != g_out) Log("badout;");
    sprintf(g_buf, "lout%ld;", value); Log(g_buf);
}

static unsigned char g_ctxStore[sizeof(CPersistContext)];
static unsigned char g_inStore[64], g_outStore[64];
static CPersistContext& Context(int mode)
{
    CPersistContext& c = *reinterpret_cast<CPersistContext*>(g_ctxStore);
    memset(g_ctxStore, 0, sizeof(g_ctxStore));
    c.Mode = (CPersistContext::EMode)mode;
    c.PLoadStream = reinterpret_cast<CMemoryDataInputStream*>(g_inStore);
    c.PSaveStream = reinterpret_cast<CDataOutputStream*>(g_outStore);
    g_in = g_inStore; g_out = g_outStore;
    g_log[0] = 0;
    return c;
}

int main()
{
    static long values[2];
    g_base = values;
    values[1] = 1234;
    Context(CPersistContext::MODE_BINARY_LOAD).TransferBinaryMember(values[1]);
    if (strcmp(g_log, "crc;lin4;") != 0 || values[1] != 77) { printf("load: %s\n", g_log); return 1; }
    values[1] = -5;
    Context(CPersistContext::MODE_BINARY_SAVE).TransferBinaryMember(values[1]);
    if (strcmp(g_log, "crc;lout-5;") != 0) { printf("save: %s\n", g_log); return 1; }
    for (int mode = 0; mode <= 4; ++mode)
    {
        if (mode == 2 || mode == 3) continue;
        Context(mode).TransferBinaryMember(values[1]);
        if (strcmp(g_log, "crc;") != 0) { printf("mode %d: %s\n", mode, g_log); return 1; }
    }
    puts("FABLETLC_CPERSISTCONTEXT_TRANSFERBINARYMEMBER_LONG_00431102_PASS");
    return 0;
}
