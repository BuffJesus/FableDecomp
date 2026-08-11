#include <cstdio>

struct Pair {
    unsigned long first;
    unsigned long second;
    void destroy();
};
struct Alloc { char dummy; };

void __fastcall Destroy_range_allocator(Pair* first, Pair* last, Alloc* al);

static int g_count = 0;
static unsigned long g_sum = 0;
void Pair::destroy() { g_count++; g_sum += first; }

int main() {
    Pair arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].first = (unsigned long)(i + 1); arr[i].second = 0; }
    Alloc a;
    Destroy_range_allocator(&arr[0], &arr[5], &a);
    // expect 5 destroy calls, sum 1+2+3+4+5=15
    // also test empty range: no calls
    int before = g_count;
    Destroy_range_allocator(&arr[0], &arr[0], &a);
    if (g_count == 5 && g_sum == 15 && before == 5) {
        printf("DESTROY_RANGE_OK count=%d sum=%lu\n", g_count, g_sum);
        return 0;
    }
    printf("DESTROY_RANGE_FAIL count=%d sum=%lu\n", g_count, g_sum);
    return 1;
}