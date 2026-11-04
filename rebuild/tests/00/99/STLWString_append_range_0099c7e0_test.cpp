// Behaviour of basic_string<wchar_t>::append(first, last, forward_iterator_tag)
// 0x0099C7E0: an empty range changes nothing; a range that fits after the old
// text is copied in place (the old terminator overwritten, a new one after);
// otherwise a buffer of size + max(size, n) + 1 characters receives both, the
// old buffer is freed and the capacity grows to that size minus the terminator.
#include "fable_stlport_string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

typedef _STL::basic_string<wchar_t> WStr;

void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const
{
    puts("length error");
    exit(1);
}

static bool Is(const WStr& s, const wchar_t* text, size_t capacity)
{
    return wcscmp(s.c_str(), text) == 0 && s.size() == wcslen(text) && s.capacity() == capacity &&
        s.c_str()[s.size()] == 0;
}

int main()
{
    // "abc": allocated for exactly 3 + 1.
    WStr s(L"abc");
    if (!Is(s, L"abc", 3)) { puts("ctor"); return 1; }
    const wchar_t* more = L"defgh";
    // Empty range: nothing.
    const wchar_t* before = s.c_str();
    s.append(more, more, _STL::forward_iterator_tag());
    if (s.c_str() != before || !Is(s, L"abc", 3)) { puts("empty"); return 1; }
    // Two more do not fit: 3 + max(3, 2) + 1 = 7 characters, capacity 6.
    s.append(more, more + 2, _STL::forward_iterator_tag());
    if (s.c_str() == before || !Is(s, L"abcde", 6)) { puts("grow"); return 1; }
    // One more fits in place.
    const wchar_t* grown = s.c_str();
    s.append(more + 2, more + 3, _STL::forward_iterator_tag());
    if (s.c_str() != grown || !Is(s, L"abcdef", 6)) { puts("fit"); return 1; }
    // Five more: 6 + max(6, 5) + 1 = 13, capacity 12.
    s.append(more, more + 5, _STL::forward_iterator_tag());
    if (!Is(s, L"abcdefdefgh", 12)) { puts("grow2"); return 1; }
    // One more (fits: 11 + 1 <= 12), then a longer one than the old size.
    s.append(more, more + 1, _STL::forward_iterator_tag());
    if (!Is(s, L"abcdefdefghd", 12)) { puts("fit2"); return 1; }
    WStr t(L"x");
    t.append(more, more + 5, _STL::forward_iterator_tag());
    if (!Is(t, L"xdefgh", 6)) { puts("grow3"); return 1; }
    puts("FABLETLC_STL_WSTRING_APPEND_0099C7E0_PASS");
    return 0;
}