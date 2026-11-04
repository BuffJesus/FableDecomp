// Behaviour of vector<CWideString>::operator= 0x00433DF0: self-assignment is a
// no-op; a source longer than the capacity is copied into new storage and the
// old strings destroyed and freed; otherwise the common prefix is assigned,
// then the surplus destroyed (shorter source) or the rest copy-constructed
// (longer source) in place.
#include "fable_string.h"
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef _STL::vector<CWideString> Vec;

static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); }
static const void* g_this[8];
static const void* g_from[8];
static int g_calls;
static void Note(const void* self, const void* from)
{
    if (g_calls < 8) { g_this[g_calls] = self; g_from[g_calls] = from; }
    ++g_calls;
}
static void Reset() { g_log[0] = 0; g_calls = 0; }

CWideString::CWideString(const CWideString& other) { Log("copy;"); Note(this, &other); }
CWideString::~CWideString() { Log("dtor;"); Note(this, 0); }
CWideString& CWideString::operator=(const CWideString& other) { Log("assign;"); Note(this, &other); return *this; }
void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const {}

static void Set(Vec& v, CWideString* start, int size, int capacity)
{
    v._M_start = start;
    v._M_finish = start + size;
    v._M_end_of_storage._M_data = start + capacity;
}

int main()
{
    static unsigned char dstStore[sizeof(Vec)];
    static unsigned char srcStore[sizeof(Vec)];
    Vec& dst = *reinterpret_cast<Vec*>(dstStore);
    Vec& src = *reinterpret_cast<Vec*>(srcStore);
    CWideString* from = reinterpret_cast<CWideString*>(malloc(3 * sizeof(CWideString)));
    Set(src, from, 3, 3);

    // Self-assignment touches nothing.
    CWideString* own = reinterpret_cast<CWideString*>(malloc(4 * sizeof(CWideString)));
    Set(dst, own, 2, 4);
    Reset();
    if (&(dst = dst) != &dst || g_log[0] != 0 || dst._M_start != own || dst._M_finish != own + 2)
    { printf("self: %s\n", g_log); return 1; }

    // Longer than the capacity: copy into new storage, destroy and free the old.
    CWideString* small = reinterpret_cast<CWideString*>(malloc(1 * sizeof(CWideString)));
    Set(dst, small, 1, 1);
    Reset();
    if (&(dst = src) != &dst) { puts("grow: result"); return 1; }
    if (strcmp(g_log, "copy;copy;copy;dtor;") != 0 || dst._M_start == small || dst._M_start == 0 ||
        dst._M_finish != dst._M_start + 3 || dst._M_end_of_storage._M_data != dst._M_start + 3 ||
        g_this[0] != dst._M_start || g_from[0] != from || g_this[2] != dst._M_start + 2 || g_from[2] != from + 2 ||
        g_this[3] != small)
    { printf("grow: %s\n", g_log); return 1; }
    free(dst._M_start);

    // Shorter source: assign the prefix, destroy the surplus in place.
    CWideString* big = reinterpret_cast<CWideString*>(malloc(5 * sizeof(CWideString)));
    Set(dst, big, 5, 5);
    Set(src, from, 2, 3);
    Reset();
    dst = src;
    if (strcmp(g_log, "assign;assign;dtor;dtor;dtor;") != 0 || dst._M_start != big || dst._M_finish != big + 2 ||
        dst._M_end_of_storage._M_data != big + 5 || g_this[1] != big + 1 || g_from[1] != from + 1 ||
        g_this[2] != big + 2 || g_this[4] != big + 4)
    { printf("shrink: %s\n", g_log); return 1; }

    // Longer source that fits: assign over the live strings, construct the rest.
    Set(dst, big, 1, 5);
    Set(src, from, 3, 3);
    Reset();
    dst = src;
    if (strcmp(g_log, "assign;copy;copy;") != 0 || dst._M_start != big || dst._M_finish != big + 3 ||
        dst._M_end_of_storage._M_data != big + 5 || g_this[0] != big || g_from[0] != from ||
        g_this[1] != big + 1 || g_from[1] != from + 1 || g_this[2] != big + 2 || g_from[2] != from + 2)
    { printf("fill: %s\n", g_log); return 1; }

    puts("FABLETLC_STLVECTOR_CWIDESTRING_ASSIGN_00433DF0_PASS");
    return 0;
}