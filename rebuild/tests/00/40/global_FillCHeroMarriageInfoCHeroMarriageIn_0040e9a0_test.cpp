#include <cstdio>
#include <cstring>

struct CHeroMarriageInfo {
    int a, b, c, d, e, f, g;
};

void __fastcall Fill(CHeroMarriageInfo* _First, CHeroMarriageInfo* _Last, const CHeroMarriageInfo& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}

int main()
{
    CHeroMarriageInfo dst[4];
    std::memset(dst, 0, sizeof(dst));
    CHeroMarriageInfo v;
    for (int i = 0; i < 7; ++i) ((int*)&v)[i] = 100 + i;

    Fill(dst, dst + 4, v);

    for (int k = 0; k < 4; ++k) {
        for (int i = 0; i < 7; ++i) {
            if (((int*)&dst[k])[i] != 100 + i) {
                std::printf("FAIL at %d,%d\n", k, i);
                return 1;
            }
        }
    }

    // empty-range: no writes
    CHeroMarriageInfo one[1];
    std::memset(one, 0x55, sizeof(one));
    Fill(one, one, v);
    if (one[0].a != 0x55555555) { std::printf("FAIL empty\n"); return 1; }

    std::printf("_global_0040e9a0_TEST PASS\n");
    return 0;
}