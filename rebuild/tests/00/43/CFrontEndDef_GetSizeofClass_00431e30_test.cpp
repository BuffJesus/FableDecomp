#include "fable_frontend_def.h"
#include <stdio.h>

int main()
{
    unsigned char storage[0x54] = {0};
    CFrontEndDef* definition = reinterpret_cast<CFrontEndDef*>(storage);
    if (sizeof(CFrontEndDef) != 0x54 ||
        definition->CFrontEndDef::GetSizeofClass() != 0x54)
        return 1;
    puts("PASS_00431e30");
    return 0;
}
