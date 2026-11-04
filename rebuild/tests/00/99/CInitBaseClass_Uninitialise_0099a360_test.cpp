#include <stdio.h>
#include <string.h>
#include "fable_init_base_class.h"

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;

struct UninitialiseProbe : CInitBaseClass
{
    void Call() { Uninitialise(); }
};

int main()
{
    // Retail compares the byte with 1 exactly; other nonzero bytes stay.
    const unsigned char before[] = { 1, 0, 2, 0xFF };
    const unsigned char after[] = { 0, 0, 2, 0xFF };
    const long counter[] = { 8, 9, 9, 9 };
    for (int i = 0; i < 4; ++i)
    {
        unsigned char storage[8];
        memset(storage, 0x5A, sizeof(storage));
        storage[4] = before[i];
        g_CInitBaseClassValid_013BCA14 = 9;
        g_CInitBaseClassConstructed_013BCA18 = 4;
        reinterpret_cast<UninitialiseProbe*>(storage)->Call();
        if (storage[4] != after[i]) return 1 + i;
        if (g_CInitBaseClassValid_013BCA14 != counter[i]) return 10 + i;
        if (g_CInitBaseClassConstructed_013BCA18 != 4) return 20 + i;
        if (storage[0] != 0x5A || storage[5] != 0x5A) return 30 + i;
    }
    puts("0099a360_TEST PASS");
    return 0;
}
