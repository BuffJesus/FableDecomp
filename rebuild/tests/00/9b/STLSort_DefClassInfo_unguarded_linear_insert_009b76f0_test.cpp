// Behaviour of __unguarded_linear_insert<pair<unsigned long, CDefClassInfo>*>
// 0x009B76F0: from the given last position, entries whose key is above the
// value's move up one place (member-wise assignment) until one is not; the
// value is assigned to the freed slot, then its by-value copy is destroyed
// (its index buffer freed, its name released).
#include "fable_stlport_unguarded_linear_insert.h"
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CKeyPairCompareLess<unsigned long, Info> Less;

static char g_log[512];
static char g_buf[64];
static void Log(const char* t) { strcat(g_log, t); }

static const char* Text(const CCharString& s) { return *reinterpret_cast<const char* const*>(&s); }
CCharString& CCharString::operator=(const CCharString& other)
{
    *reinterpret_cast<const char**>(this) = Text(other);
    sprintf(g_buf, "s=%s;", Text(other)); Log(g_buf);
    return *this;
}
CCharString::~CCharString() { sprintf(g_buf, "~s%s;", Text(*this)); Log(g_buf); }
template <> _STL::vector<unsigned long>& _STL::vector<unsigned long>::operator=(const _STL::vector<unsigned long>& x)
{
    _M_start = x._M_start;
    Log("v=;");
    return *this;
}

static unsigned char g_store[5][sizeof(Entry)];
static const char* g_names[5] = { "A", "B", "C", "D", "E" };
static Entry* E(int i) { return reinterpret_cast<Entry*>(g_store[i]); }
static void Reset()
{
    memset(g_store, 0, sizeof(g_store));
    const unsigned long keys[4] = { 10, 20, 30, 40 };
    for (int i = 0; i < 4; ++i)
    {
        E(i)->first = keys[i];
        *reinterpret_cast<const char**>(&E(i)->second.Name) = g_names[i];
        E(i)->second.AllocFunc = (CDefClassInit::AllocFunction)(100 + i);
        E(i)->second.GlobalIndices._M_start = (unsigned long*)(200 + i);
    }
    g_log[0] = 0;
}

// The by-value argument, built as the caller would (raw storage: the callee owns it).
static void Call(Entry* last, unsigned long key, bool withBuffer)
{
    static unsigned char valStore[sizeof(Entry)];
    Entry& val = *reinterpret_cast<Entry*>(valStore);
    memset(valStore, 0, sizeof(valStore));
    val.first = key;
    *reinterpret_cast<const char**>(&val.second.Name) = "V";
    val.second.AllocFunc = (CDefClassInit::AllocFunction)99;
    val.second.GlobalIndices._M_start = withBuffer ? (unsigned long*)malloc(8) : 0;
    typedef void (FABLE_FASTCALL *Fn)(Entry*, Entry, Less);
    Fn fn = &_STL::__unguarded_linear_insert<Entry*, Entry, Less>;
    // Pass the raw bytes: no copy constructor runs here.
    struct Raw { unsigned char b[sizeof(Entry)]; };
    typedef void (FABLE_FASTCALL *RawFn)(Entry*, int, Raw, Less);
    RawFn raw = (RawFn)fn;
    raw(last, 0, *reinterpret_cast<Raw*>(valStore), Less());
}

int main()
{
    // Key 25 from the end (E4 free): 40 and 30 move up, the value lands in E2.
    Reset();
    Call(E(4), 25, true);
    if (strcmp(g_log, "s=D;v=;s=C;v=;s=V;v=;~sV;") != 0) { printf("middle: %s\n", g_log); return 1; }
    const unsigned long k[5] = { 10, 20, 25, 30, 40 };
    for (int i = 0; i < 5; ++i)
        if (E(i)->first != k[i]) { printf("middle key %d\n", i); return 1; }
    if ((unsigned long)E(2)->second.AllocFunc != 99 || (unsigned long)E(3)->second.AllocFunc != 102 ||
        strcmp(Text(E(4)->second.Name), "D") != 0) { puts("middle members"); return 1; }
    // Key 40 (equal to the last): stays at the end, nothing moves.
    Reset();
    Call(E(4), 40, false);
    if (strcmp(g_log, "s=V;v=;~sV;") != 0 || E(4)->first != 40 || E(3)->first != 40) { printf("equal: %s\n", g_log); return 1; }
    // Key 15: moves down to E1.
    Reset();
    Call(E(4), 15, false);
    if (strcmp(g_log, "s=D;v=;s=C;v=;s=B;v=;s=V;v=;~sV;") != 0 || E(1)->first != 15 || E(2)->first != 20 || E(0)->first != 10)
    { printf("low: %s\n", g_log); return 1; }
    puts("FABLETLC_STL_UNGUARDED_LINEAR_INSERT_DEFCLASSINFO_009B76F0_PASS");
    return 0;
}