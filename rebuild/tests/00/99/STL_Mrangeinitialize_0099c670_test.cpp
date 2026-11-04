// Behaviour of the landed basic_string<wchar_t> range initialiser 0x0099C670:
// allocates length + 1 characters from malloc, copies the range, terminates it,
// sets start/finish/end-of-storage, and reports an impossible length through
// _M_throw_length_error without allocating.
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// Declaration-only view of fable_stlport_string.h (same mangled names), so
// the call binds to the landed instantiation instead of a local COMDAT copy.
namespace _STL
{
    struct input_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    template <class _Tp> class allocator {};
    template <class _CharT> struct char_traits {};
    template <class _Tp, class _Alloc>
    class _String_base
    {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        struct { _Tp* _M_data; } _M_end_of_storage;
        __declspec(noreturn) void _M_throw_length_error() const;
    };
    template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
    class basic_string : public _String_base<_CharT, _Alloc>
    {
    public:
        template <class _ForwardIter>
        void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&);
    };
}

static jmp_buf g_jump;
static const void* g_throwThis;
static int g_throws;
void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const
{
    g_throwThis = this;
    ++g_throws;
    longjmp(g_jump, 1);
}

typedef _STL::basic_string<wchar_t> WS;

static bool Check(const wchar_t* first, size_t count)
{
    unsigned long raw[3] = {0, 0, 0};
    WS& s = *reinterpret_cast<WS*>(raw);
    s._M_range_initialize<const wchar_t*>(first, first + count, _STL::random_access_iterator_tag());
    if (s._M_start == 0 || s._M_start == first) return false;
    if (s._M_finish != s._M_start + count) return false;
    if (s._M_end_of_storage._M_data != s._M_start + count + 1) return false;
    if (memcmp(s._M_start, first, count * sizeof(wchar_t)) != 0) return false;
    if (s._M_start[count] != 0) return false;
    free(s._M_start);
    return true;
}

int main()
{
    const wchar_t text[] = L"xfrontend.bin";
    if (!Check(text, wcslen(text))) { puts("copy"); return 1; }
    if (!Check(text + 1, 3)) { puts("partial"); return 1; }
    if (!Check(text, 0)) { puts("empty"); return 1; }

    // A range longer than max_size(): n + 1 > 0x7FFFFFFF.
    unsigned long raw[3] = {0, 0, 0};
    WS& s = *reinterpret_cast<WS*>(raw);
    if (setjmp(g_jump) == 0)
    {
        s._M_range_initialize<const wchar_t*>(text, text + 0x7FFFFFFF, _STL::random_access_iterator_tag());
        puts("no throw");
        return 1;
    }
    if (g_throws != 1 || g_throwThis != &s || raw[0] != 0 || raw[1] != 0 || raw[2] != 0)
    {
        puts("throw state");
        return 1;
    }
    puts("FABLETLC_STL_WSTRING_RANGE_INITIALIZE_0099C670_PASS");
    return 0;
}