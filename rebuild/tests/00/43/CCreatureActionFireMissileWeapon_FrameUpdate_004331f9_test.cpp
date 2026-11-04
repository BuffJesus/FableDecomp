// Behaviour of CPersistContext's binary-only vector transfer <CCharString>
// 0x004331F9: an unnamed CRC check, then GFSerialiseVectorBinaryIn from the
// load stream when loading, otherwise GFSerialiseVectorBinaryOut to the save
// stream (every other mode included).
#include "fable_persist_context.h"
#include "fable_string.h"
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
    static unsigned char vecStore[sizeof(_STL::vector<CCharString>) + 4];
    _STL::vector<CCharString>& v = *reinterpret_cast<_STL::vector<CCharString>*>(vecStore + 4);
    g_base = vecStore;
    Context(CPersistContext::MODE_BINARY_LOAD).TransferBinaryVector(v);
    if (strcmp(g_log, "crc;vin4;") != 0) { printf("load: %s\n", g_log); return 1; }
    Context(CPersistContext::MODE_BINARY_SAVE).TransferBinaryVector(v);
    if (strcmp(g_log, "crc;vout4;") != 0) { printf("save: %s\n", g_log); return 1; }
    Context(CPersistContext::MODE_TEXT_LOAD).TransferBinaryVector(v);
    if (strcmp(g_log, "crc;vout4;") != 0) { printf("text: %s\n", g_log); return 1; }
    puts("FABLETLC_CPERSISTCONTEXT_TRANSFERBINARYVECTOR_CCHARSTRING_004331F9_PASS");
    return 0;
}
