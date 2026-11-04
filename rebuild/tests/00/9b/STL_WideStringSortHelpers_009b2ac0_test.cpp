#include <stdio.h>
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_linear_insert.h"

typedef _STL::less<CWideString> WideLess;
static int g_partitions, g_partials;

static unsigned long& Key(CWideString& value) {
    return *reinterpret_cast<unsigned long*>(&value);
}
static unsigned long Key(const CWideString& value) {
    return *reinterpret_cast<const unsigned long*>(&value);
}
static void SortKeys(CWideString* first, CWideString* last) {
    for (CWideString* a = first; a != last; ++a)
        for (CWideString* b = a + 1; b != last; ++b)
            if (Key(*a) > Key(*b)) {
                unsigned long temp = Key(*a); Key(*a) = Key(*b); Key(*b) = temp;
            }
}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}
CWideString& CWideString::operator=(const CWideString& other) {
    storage_ = other.storage_; return *this;
}
bool CWideString::operator<(const CWideString& other) const { return Key(*this) < Key(other); }

namespace _STL {
    template <> CWideString* FABLE_FASTCALL __unguarded_partition<CWideString*, CWideString, WideLess>(
        CWideString* first, CWideString* last, CWideString pivot, WideLess) {
        ++g_partitions;
        // Keep the partition contract; sorting here isolates introsort's
        // recursion/depth routing from the partition provider's own body.
        unsigned long low = Key(*first), high = low;
        for (CWideString* p = first; p != last; ++p) {
            if (Key(*p) < low) low = Key(*p);
            if (Key(*p) > high) high = Key(*p);
        }
        if (Key(pivot) < low || Key(pivot) > high) g_partitions = -100;
        SortKeys(first, last);
        return first + (last - first) / 2;
    }
    template <> void FABLE_FASTCALL __partial_sort<CWideString*, CWideString, WideLess>(
        CWideString* first, CWideString*, CWideString* last, CWideString*, WideLess) {
        ++g_partials; SortKeys(first, last);
    }
}

static bool Sorted(const unsigned long* keys, int count) {
    for (int i = 1; i < count; ++i) if (keys[i - 1] > keys[i]) return false;
    return true;
}
static bool InsertCase(const unsigned long* input, int count) {
    unsigned long keys[16] = { 0 };
    for (int i = 0; i < count; ++i) keys[i + 1] = input[i];
    CWideString* first = reinterpret_cast<CWideString*>(keys);
    _STL::__unguarded_insertion_sort_aux(first + 1, first + count + 1,
                                         static_cast<CWideString*>(0), WideLess());
    return keys[0] == 0 && Sorted(keys + 1, count);
}
static bool BoundedInsertCase(const unsigned long* input, int count) {
    unsigned long keys[16] = { 0 };
    for (int i = 0; i < count; ++i) keys[i] = input[i];
    CWideString* first = reinterpret_cast<CWideString*>(keys);
    _STL::__insertion_sort(first, first + count, WideLess());
    return Sorted(keys, count);
}
static bool IntroCase(int count, int depth, int expectedPartitions, int expectedPartials) {
    unsigned long keys[48] = { 0 };
    for (int i = 0; i < count; ++i) keys[i] = count - i;
    CWideString* first = reinterpret_cast<CWideString*>(keys);
    g_partitions = g_partials = 0;
    _STL::__introsort_loop(first, first + count, static_cast<CWideString*>(0),
                           depth, WideLess());
    if (g_partitions != expectedPartitions || g_partials != expectedPartials)
        return false;
    return expectedPartitions || expectedPartials ? Sorted(keys, count) : keys[0] == (unsigned long)count;
}

int main() {
    const unsigned long ordered[] = { 1, 2, 3, 4, 5 };
    const unsigned long reverse[] = { 5, 4, 3, 2, 1 };
    const unsigned long duplicates[] = { 3, 1, 3, 2, 1 };
    if (!InsertCase(ordered, 5) || !InsertCase(reverse, 5) || !InsertCase(duplicates, 5)) return 1;
    if (!BoundedInsertCase(ordered, 5) || !BoundedInsertCase(reverse, 5) ||
        !BoundedInsertCase(duplicates, 5)) return 5;
    if (!IntroCase(16, 3, 0, 0)) return 2;
    if (!IntroCase(17, 0, 0, 1)) return 3;
    if (!IntroCase(20, 4, 1, 0)) return 4;
    puts("WIDESTRING_SORT_HELPERS_OK");
    return 0;
}
