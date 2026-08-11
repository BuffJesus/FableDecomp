#include <stdio.h>

struct Elem {
    unsigned long first;
    unsigned long second;
};
struct Alloc { char dummy; };

void __fastcall Destroy_pair(Elem* p);
void __fastcall Destroy_range(Elem* first, Elem* last, Alloc* al);

static int g_count = 0;
static unsigned long g_sum = 0;

void __fastcall Destroy_pair(Elem* p)
{
    g_count++;
    g_sum += p->first + p->second;
}

int main()
{
    Elem arr[4];
    for (int i = 0; i < 4; ++i) { arr[i].first = i + 1; arr[i].second = 10; }
    Alloc a;

    Destroy_range(arr, arr + 4, &a);
    // expect 4 calls, sum = (1+2+3+4) + 4*10 = 10 + 40 = 50
    if (g_count == 4 && g_sum == 50) {
        // empty range must be a no-op
        g_count = 0;
        Destroy_range(arr, arr, &a);
        if (g_count == 0) {
            printf("DESTROY_RANGE_OK\n");
            return 0;
        }
    }
    printf("FAIL count=%d sum=%lu\n", g_count, g_sum);
    return 1;
}