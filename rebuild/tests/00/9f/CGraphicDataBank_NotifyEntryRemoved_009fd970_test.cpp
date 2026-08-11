#include <stdio.h>
#include <string.h>

struct CGraphicDataBank {
    char pad[0x1e0];
    char* f1e0;
};

void __fastcall NotifyEntryRemoved(CGraphicDataBank* self, int edx, unsigned long idx);

int main()
{
    CGraphicDataBank obj;
    memset(&obj, 0, sizeof(obj));
    char arr[0x2c * 8];
    memset(arr, 0, sizeof(arr));
    obj.f1e0 = arr;

    unsigned long idx = 3;
    NotifyEntryRemoved(&obj, 0, idx);

    int v = *(int*)(arr + idx * 0x2c + 0x24);
    int other = *(int*)(arr + 2 * 0x2c + 0x24);
    if (v == -1 && other == 0)
        printf("009fd970_TEST PASS\n");
    else
        printf("FAIL v=%d other=%d\n", v, other);
    return 0;
}