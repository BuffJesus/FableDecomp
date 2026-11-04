// Behaviour: IsValid reports the Valid byte at +0x04 and reads nothing else.
#include "fable_init_base_class.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char storage[sizeof(CInitBaseClass)];
    memset(storage, 0xEE, sizeof(storage));
    const CInitBaseClass* object = reinterpret_cast<const CInitBaseClass*>(storage);
    storage[4] = 0;
    if (object->IsValid() != false) { printf("FAIL false\n"); return 1; }
    storage[4] = 1;
    if (object->IsValid() != true) { printf("FAIL true\n"); return 2; }
    storage[4] = 0; storage[0] = 1; storage[5] = 1;
    if (object->IsValid() != false) { printf("FAIL neighbours\n"); return 3; }
    printf("OK_0099a330\n");
    return 0;
}