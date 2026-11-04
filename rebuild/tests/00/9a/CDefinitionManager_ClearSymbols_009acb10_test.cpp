#include <stdio.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_symbol_map.h"

static CSymbolMap* expected;
static unsigned calls;
static bool rightMap;

void CSymbolMap::Clear()
{
    ++calls;
    rightMap = this == expected;
}

int main()
{
    unsigned char managerBytes[sizeof(CDefinitionManager)];
    unsigned char symbolBytes[sizeof(CSymbolMap)];
    unsigned char otherBytes[sizeof(CSymbolMap)];
    memset(managerBytes, 0, sizeof(managerBytes));
    CDefinitionManager& manager = *reinterpret_cast<CDefinitionManager*>(managerBytes);
    expected = reinterpret_cast<CSymbolMap*>(symbolBytes);
    memcpy(managerBytes + 0x58, &expected, sizeof(expected));
    CSymbolMap* other = reinterpret_cast<CSymbolMap*>(otherBytes);
    memcpy(managerBytes + 0x60, &other, sizeof(other));

    manager.ClearSymbols();
    if (calls != 1 || !rightMap) return 1;
    expected = reinterpret_cast<CSymbolMap*>(otherBytes);
    memcpy(managerBytes + 0x58, &expected, sizeof(expected));
    manager.ClearSymbols();
    if (calls != 2 || !rightMap) return 2;

    puts("DEFINITION_CLEAR_SYMBOLS_PASS");
    return 0;
}
