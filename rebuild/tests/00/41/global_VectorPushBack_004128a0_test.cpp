// Behaviour of vector<CWideString>::push_back 0x004128A0: with a spare slot the
// string is copy-constructed at the finish, which moves up one; a full vector
// grows by reallocation (STLport _M_insert_overflow).
#include "fable_string.h"
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef _STL::vector<CWideString> Vec;

static char g_log[128];
static void Log(const char* t) { strcat(g_log, t); }
static const void* g_copyThis;
static const void* g_copyFrom;
CWideString::CWideString(const CWideString& other) { Log("copy;"); g_copyThis = this; g_copyFrom = &other; }
static int g_destroyed;
CWideString::~CWideString() { ++g_destroyed; }
void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const {}

int main()
{
    static unsigned char vecStore[sizeof(Vec)];
    unsigned char* slots = (unsigned char*)malloc(3 * sizeof(CWideString));
    static unsigned char xStore[sizeof(CWideString)];
    Vec& v = *reinterpret_cast<Vec*>(vecStore);
    const CWideString& x = *reinterpret_cast<const CWideString*>(xStore);
    CWideString* base = reinterpret_cast<CWideString*>(slots);
    v._M_start = base;
    v._M_finish = base + 1;
    v._M_end_of_storage._M_data = base + 3;
    v.push_back(x);
    if (strcmp(g_log, "copy;") != 0 || g_copyThis != base + 1 || g_copyFrom != &x || v._M_finish != base + 2)
    { printf("spare: %s\n", g_log); return 1; }
    // Full: the storage grows (the old strings copied across, the new one
    // appended, the old ones destroyed and their block freed).
    g_log[0] = 0;
    v._M_finish = base + 3;
    v.push_back(x);
    if (strcmp(g_log, "copy;copy;copy;copy;") != 0 || g_copyFrom != &x || v._M_start == base ||
        v._M_finish != v._M_start + 4 || g_destroyed != 3)
    { printf("full: %s %d\n", g_log, g_destroyed); return 1; }
    puts("FABLETLC_STLVECTOR_CWIDESTRING_PUSH_BACK_004128A0_PASS");
    return 0;
}
