// Behaviour of CWideString::AllocStringData 0x0099B3C0: one 16-byte block per
// call, the whole C string for length -1 or a prefix otherwise, copied into a
// fresh terminated buffer (start/finish/end of storage) with one owner.
#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static int g_blocks;
static size_t g_lastSize;
void* operator new(size_t size)
{
    ++g_blocks;
    g_lastSize = size;
    return malloc(size);
}
void operator delete(void* p) { free(p); }

static int g_throws;
void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const
{
    ++g_throws;
    abort();
}

// AllocStringData is private: take its address through an explicit
// instantiation (which skips access checks), keeping the real mangled name.
typedef CWideStringData* (CWideString::*AllocFn)(const wchar_t*, long);
static AllocFn g_alloc;
template <AllocFn F> struct Grab
{
    Grab() { g_alloc = F; }
    static Grab instance;
};
template <AllocFn F> Grab<F> Grab<F>::instance;
template struct Grab<&CWideString::AllocStringData>;

struct Probe
{
    static CWideStringData* Alloc(CWideString& owner, const wchar_t* text, long length)
    {
        return (owner.*g_alloc)(text, length);
    }
};

static bool Check(CWideStringData* data, const wchar_t* text, size_t count)
{
    return data != 0 && data->Data._M_start != 0 && data->Data._M_start != text &&
        data->Data._M_finish == data->Data._M_start + count &&
        data->Data._M_end_of_storage._M_data == data->Data._M_start + count + 1 &&
        memcmp(data->Data._M_start, text, count * sizeof(wchar_t)) == 0 &&
        data->Data._M_start[count] == 0 && data->owners == 1;
}

int main()
{
    unsigned long ownerRaw = 0x5A5A5A5A;
    CWideString& owner = *reinterpret_cast<CWideString*>(&ownerRaw);
    const wchar_t text[] = L"frontend.bin";

    CWideStringData* whole = Probe::Alloc(owner, text, -1);
    if (g_blocks != 1 || g_lastSize != 0x10 || !Check(whole, text, wcslen(text))) { puts("whole"); return 1; }
    CWideStringData* prefix = Probe::Alloc(owner, text, 5);
    if (g_blocks != 2 || g_lastSize != 0x10 || !Check(prefix, text, 5)) { puts("prefix"); return 1; }
    CWideStringData* empty = Probe::Alloc(owner, text, 0);
    if (g_blocks != 3 || !Check(empty, text, 0)) { puts("empty"); return 1; }
    if (ownerRaw != 0x5A5A5A5A || g_throws != 0) { puts("owner touched"); return 1; }
    puts("FABLETLC_CWIDESTRING_ALLOCSTRINGDATA_0099B3C0_PASS");
    return 0;
}