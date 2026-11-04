// Behaviour of _STL::__introsort_loop<pair<unsigned long, CDefClassInfo>*, ..., int,
// CKeyPairCompareLess<...> > 0x009B9320: runs of 16 or fewer 24-byte entries are
// left alone; with the depth limit spent the range goes to __partial_sort(first,
// last, last); otherwise the median of first / middle / last-1 by key is copied
// as the pivot (name and index list copied too), __unguarded_partition splits the
// range, the right part recurses with depth-1 and the loop continues on the left.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer.
static int g_copies;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() {}

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CKeyPairCompareLess<unsigned long, Info> Less;
static double g_raw[64 * sizeof(Entry) / sizeof(double)];
#define g_a (reinterpret_cast<Entry*>(g_raw))
static char g_log[512];
static int g_len;
static int g_cuts[4];
static int g_cutIndex;
template<> Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry* first, Entry* last, Entry pivot, Less)
{
    g_len += sprintf(g_log + g_len, "P%d,%d,%lu,%lx;", (int)(first - g_a), (int)(last - g_a), pivot.first,
                     *reinterpret_cast<unsigned long*>(&pivot.second.Name));
    return g_a + g_cuts[g_cutIndex++];
}
template<> void FABLE_FASTCALL _STL::__partial_sort(Entry* first, Entry* middle, Entry* last, Entry* tag, Less)
{
    g_len += sprintf(g_log + g_len, "H%d,%d,%d,%d;", (int)(first - g_a), (int)(middle - g_a), (int)(last - g_a), tag == 0);
}

static int Run(int first, int last, int depth, const char* want)
{
    g_len = 0; g_log[0] = 0; g_cutIndex = 0;
    _STL::__introsort_loop(g_a + first, g_a + last, (Entry*)0, depth, Less());
    if (strcmp(g_log, want) != 0) { printf("got %s want %s\n", g_log, want); return 1; }
    return 0;
}
static void Keys(int n, unsigned long a, unsigned long b, unsigned long c)
{
    g_a[0].first = a; *reinterpret_cast<unsigned long*>(&g_a[0].second.Name) = 0xA;
    g_a[n / 2].first = b; *reinterpret_cast<unsigned long*>(&g_a[n / 2].second.Name) = 0xB;
    g_a[n - 1].first = c; *reinterpret_cast<unsigned long*>(&g_a[n - 1].second.Name) = 0xC;
}

int main()
{
    memset(g_raw, 0, sizeof(g_raw));
    if (Run(0, 16, 5, "")) return 1;
    if (Run(0, 20, 0, "H0,20,20,1;")) return 1;
    g_cuts[0] = 10;
    Keys(40, 5, 9, 7);
    if (Run(0, 40, 1, "P0,40,7,c;H10,40,40,1;")) return 1;
    Keys(40, 5, 3, 7);
    if (Run(0, 40, 1, "P0,40,5,a;H10,40,40,1;")) return 1;
    Keys(40, 5, 6, 7);
    if (Run(0, 40, 1, "P0,40,6,b;H10,40,40,1;")) return 1;
    Keys(40, 9, 3, 7);
    if (Run(0, 40, 1, "P0,40,7,c;H10,40,40,1;")) return 1;
    Keys(40, 9, 8, 7);
    if (Run(0, 40, 1, "P0,40,8,b;H10,40,40,1;")) return 1;
    g_cuts[0] = 34; g_cuts[1] = 10;
    Keys(40, 1, 2, 3);
    if (Run(0, 40, 2, "P0,40,2,b;P0,34,0,0;H10,34,34,1;")) return 1;
    if (g_copies != 7) { printf("copies %d\n", g_copies); return 1; }
    puts("FABLETLC_STL_INTROSORT_LOOP_DEFCLASSENTRY_009B9320_PASS");
    return 0;
}