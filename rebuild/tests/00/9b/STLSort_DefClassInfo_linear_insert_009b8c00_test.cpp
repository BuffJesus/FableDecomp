// Behaviour of __linear_insert<pair<unsigned long, CDefClassInfo>*> 0x009B8C00:
// a value whose key is below the first entry's shifts [first, last) up one
// place (member-wise assignment, last to first) and is assigned to the first
// entry; otherwise the value (copied) and the comparator go to
// __unguarded_linear_insert with the last position. The by-value argument is
// destroyed on return.
#include "fable_stlport_linear_insert.h"
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CKeyPairCompareLess<unsigned long, Info> Less;

static char g_log[512];
static char g_buf[64];
static void Log(const char* t) { strcat(g_log, t); }

// A string is its text pointer; a vector its start pointer is enough here.
static const char* Text(const CCharString& s) { return *reinterpret_cast<const char* const*>(&s); }
CCharString& CCharString::operator=(const CCharString& other)
{
    *reinterpret_cast<const char**>(this) = Text(other);
    sprintf(g_buf, "s=%s;", Text(other)); Log(g_buf);
    return *this;
}
CCharString::CCharString(const CCharString& other)
{
    *reinterpret_cast<const char**>(this) = Text(other);
    sprintf(g_buf, "scopy%s;", Text(other)); Log(g_buf);
}
CCharString::~CCharString() { sprintf(g_buf, "~s%s;", Text(*this)); Log(g_buf); }
template <> _STL::vector<unsigned long>& _STL::vector<unsigned long>::operator=(const _STL::vector<unsigned long>& x)
{
    _M_start = x._M_start;
    Log("v=;");
    return *this;
}
template <> _STL::vector<unsigned long>::vector(const _STL::vector<unsigned long>& x)
    : _STL::_Vector_base<unsigned long, _STL::allocator<unsigned long> >(_STL::allocator<unsigned long>())
{
    _M_start = x._M_start; _M_finish = x._M_start; _M_end_of_storage._M_data = x._M_start;
    Log("vcopy;");
}
static Entry* g_ulast;
static unsigned long g_ukey;
static const char* g_uname;
template <> void FABLE_FASTCALL _STL::__unguarded_linear_insert<Entry*, Entry, Less>(Entry* last, Entry val, Less)
{
    g_ulast = last; g_ukey = val.first; g_uname = Text(val.second.Name);
    Log("unguarded;");
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

int main()
{
    static unsigned char valStore[sizeof(Entry)];
    Entry& val = *reinterpret_cast<Entry*>(valStore);
    val.first = 5;
    *reinterpret_cast<const char**>(&val.second.Name) = "V";
    val.second.AllocFunc = (CDefClassInit::AllocFunction)99;
    val.second.GlobalIndices._M_start = (unsigned long*)299;
    val.second.GlobalIndices._M_finish = val.second.GlobalIndices._M_end_of_storage._M_data = 0;

    // Below the first: [E0, E3) moves to [E1, E4), then E0 = val.
    Reset();
    _STL::__linear_insert(E(0), E(3), val, Less());
    const char* want = "scopyV;vcopy;s=C;v=;s=B;v=;s=A;v=;s=V;v=;~sV;";
    if (strcmp(g_log, want) != 0) { printf("below: %s\n", g_log); return 1; }
    for (int i = 0; i < 4; ++i)
    {
        const unsigned long k[4] = { 5, 10, 20, 30 };
        const char* n[4] = { "V", "A", "B", "C" };
        if (E(i)->first != k[i] || strcmp(Text(E(i)->second.Name), n[i]) != 0 ||
            (unsigned long)E(i)->second.AllocFunc != (i ? 99 + i : 99) ||
            (unsigned long)E(i)->second.GlobalIndices._M_start != (i ? 199 + i : 299)) { printf("below entry %d\n", i); return 1; }
    }
    // Equal to the first (not below): unguarded insert from the last position.
    Reset();
    val.first = 10;
    _STL::__linear_insert(E(0), E(3), val, Less());
    // (The callee destroys its by-value copy: the first ~sV.)
    if (strcmp(g_log, "scopyV;vcopy;scopyV;vcopy;unguarded;~sV;~sV;") != 0)
    { printf("equal: %s\n", g_log); return 1; }
    if (g_ulast != E(3) || g_ukey != 10 || strcmp(g_uname, "V") != 0 || E(0)->first != 10 || E(3)->first != 40) { puts("equal args"); return 1; }
    // Above the first.
    Reset();
    val.first = 35;
    _STL::__linear_insert(E(0), E(3), val, Less());
    if (strstr(g_log, "unguarded;") == 0 || g_ulast != E(3) || g_ukey != 35 || E(1)->first != 20) { printf("above: %s\n", g_log); return 1; }
    puts("FABLETLC_STL_LINEAR_INSERT_DEFCLASSINFO_009B8C00_PASS");
    return 0;
}