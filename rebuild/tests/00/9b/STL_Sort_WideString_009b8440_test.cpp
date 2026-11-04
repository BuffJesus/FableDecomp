#include <stdio.h>
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"

typedef _STL::less<CWideString> WideLess;
static int g_introsort, g_insertion, g_unguarded;

static unsigned long& Key(CWideString& value) {
    return *reinterpret_cast<unsigned long*>(&value);
}
static void SortKeys(CWideString* first, CWideString* last) {
    for (CWideString* a = first; a != last; ++a)
        for (CWideString* b = a + 1; b != last; ++b)
            if (Key(*a) > Key(*b)) {
                unsigned long temp = Key(*a); Key(*a) = Key(*b); Key(*b) = temp;
            }
}

namespace _STL {
    template <> void FABLE_FASTCALL __introsort_loop<CWideString*, CWideString, int, WideLess>(
        CWideString*, CWideString*, CWideString*, int, WideLess) {
        ++g_introsort;
    }
    template <> void FABLE_FASTCALL __insertion_sort<CWideString*, WideLess>(
        CWideString* first, CWideString* last, WideLess) {
        ++g_insertion; SortKeys(first, last);
    }
    template <> void FABLE_FASTCALL __unguarded_insertion_sort_aux<CWideString*, CWideString, WideLess>(
        CWideString* first, CWideString* last, CWideString*, WideLess) {
        ++g_unguarded; SortKeys(first - 16, last);
    }
}

static bool Run(int count, int intro, int insertion, int unguarded) {
    unsigned long raw[20];
    for (int i = 0; i < count; ++i) raw[i] = count - i;
    g_introsort = g_insertion = g_unguarded = 0;
    CWideString* first = reinterpret_cast<CWideString*>(raw);
    _STL::sort(first, first + count, WideLess());
    if (g_introsort != intro || g_insertion != insertion || g_unguarded != unguarded)
        return false;
    for (int i = 0; i < count; ++i) if (raw[i] != (unsigned long)i + 1) return false;
    return true;
}

int main() {
    if (!Run(0, 0, 0, 0)) return 1;
    if (!Run(1, 1, 1, 0)) return 2;
    if (!Run(2, 1, 1, 0)) return 3;
    if (!Run(20, 1, 1, 1)) return 4;
    puts("SORT_WIDESTRING_OK");
    return 0;
}
