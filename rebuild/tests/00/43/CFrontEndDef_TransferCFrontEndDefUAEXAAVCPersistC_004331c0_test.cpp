// Behaviour of CFrontEndDef::Transfer 0x004331C0: binary load and save send
// vAttractModeMovie, ErrorMessageBackgroundGraphic, ButtonABigGraphic and
// ButtonBBigGraphic, in that order, each after its (unnamed) CRC check; other
// modes only check the CRCs (the vector's non-load path saves).
#include "fable_frontend_def.h"
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
    static unsigned char defStore[sizeof(CFrontEndDef)];
    memset(defStore, 0, sizeof(defStore));
    CFrontEndDef* def = reinterpret_cast<CFrontEndDef*>(defStore);
    g_base = def;
    def->ErrorMessageBackgroundGraphic = 5; def->ButtonABigGraphic = 6; def->ButtonBBigGraphic = 7;
    def->CFrontEndDef::Transfer(Context(CPersistContext::MODE_BINARY_LOAD));
    if (strcmp(g_log, "crc;vin60;crc;lin72;crc;lin76;crc;lin80;") != 0) { printf("load: %s\n", g_log); return 1; }
    if (def->ErrorMessageBackgroundGraphic != 77 || def->ButtonBBigGraphic != 77) { puts("load values"); return 1; }
    def->ErrorMessageBackgroundGraphic = 5; def->ButtonABigGraphic = 6; def->ButtonBBigGraphic = 7;
    def->CFrontEndDef::Transfer(Context(CPersistContext::MODE_BINARY_SAVE));
    if (strcmp(g_log, "crc;vout60;crc;lout5;crc;lout6;crc;lout7;") != 0) { printf("save: %s\n", g_log); return 1; }
    def->CFrontEndDef::Transfer(Context(CPersistContext::MODE_DEFAULTS));
    if (strcmp(g_log, "crc;vout60;crc;crc;crc;") != 0) { printf("defaults: %s\n", g_log); return 1; }
    puts("FABLETLC_CFRONTENDDEF_TRANSFER_004331C0_PASS");
    return 0;
}
