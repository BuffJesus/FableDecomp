#include <stdio.h>
#include <string.h>
#include "fable_definition_manager.h"

int main()
{
    unsigned char managerBytes[sizeof(CDefinitionManager)];
    unsigned char before[sizeof(CDefinitionManager)];
    memset(managerBytes, 0xA5, sizeof(managerBytes));
    memcpy(before, managerBytes, sizeof(before));
    CDefinitionManager& manager = *reinterpret_cast<CDefinitionManager*>(managerBytes);
    manager.ScrambleDefs(false);
    manager.ScrambleDefs(true);
    if (memcmp(managerBytes, before, sizeof(before)) != 0) return 1;
    puts("DEFINITION_SCRAMBLE_PASS");
    return 0;
}
