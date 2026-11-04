#include <stdio.h>
#include <string.h>
#include "fable_init_base_class.h"

int main()
{
    unsigned char storage[8];
    unsigned char before[8];
    memset(storage, 0x6B, sizeof(storage));
    storage[4] = 1;
    memcpy(before, storage, sizeof(storage));
    const CInitBaseClass* object = reinterpret_cast<const CInitBaseClass*>(storage);
    object->CInitBaseClass::Validate();
    if (memcmp(before, storage, sizeof(storage)) != 0) return 1;
    printf("AUTO_TINY_0099a340_TEST PASS\n");
    return 0;
}
