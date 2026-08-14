#include <stdio.h>
#include <string.h>

struct CHeroMarriageInfo {
    unsigned long w[7];
};

void __fastcall Fill_CHeroMarriageInfo(CHeroMarriageInfo *_First, CHeroMarriageInfo *_Last, const CHeroMarriageInfo *_Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}

int main()
{
    CHeroMarriageInfo arr[4];
    memset(arr, 0, sizeof(arr));
    CHeroMarriageInfo val;
    for (int i = 0; i < 7; ++i) val.w[i] = 0x11111111u * (i + 1);

    Fill_CHeroMarriageInfo(arr, arr + 4, &val);

    int ok = 1;
    for (int e = 0; e < 4; ++e)
        for (int i = 0; i < 7; ++i)
            if (arr[e].w[i] != 0x11111111u * (i + 1)) ok = 0;

    // empty range no-op
    CHeroMarriageInfo one; memset(&one, 0xAB, sizeof(one));
    CHeroMarriageInfo saved = one;
    Fill_CHeroMarriageInfo(&one, &one, &val);
    if (memcmp(&one, &saved, sizeof(one)) != 0) ok = 0;

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}