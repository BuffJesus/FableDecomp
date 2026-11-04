#include <stdio.h>
#include <string.h>
#include "fable_init_base_class.h"

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;

struct InitialiseProbe : CInitBaseClass
{
    bool Call() { return Initialise(); }
};

int main()
{
    unsigned char storage[8];
    for (int start = 0; start < 3; ++start)
    {
        memset(storage, 0xA5, sizeof(storage));
        storage[4] = static_cast<unsigned char>(start);
        g_CInitBaseClassConstructed_013BCA18 = 3;
        g_CInitBaseClassValid_013BCA14 = 5;
        bool result = reinterpret_cast<InitialiseProbe*>(storage)->Call();
        if (!result) return 1;
        if (storage[4] != 1) return 2;
        if (storage[0] != 0xA5 || storage[5] != 0xA5 || storage[7] != 0xA5) return 3;
        // Retail Initialise does not touch either counter.
        if (g_CInitBaseClassConstructed_013BCA18 != 3 || g_CInitBaseClassValid_013BCA14 != 5)
            return 4;
    }
    puts("FABLETLC_INITBASE_INITIALISE PASS");
    return 0;
}