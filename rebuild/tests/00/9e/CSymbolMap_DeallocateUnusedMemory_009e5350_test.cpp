#include <stdio.h>
#include <string.h>
#include "fable_symbol_map.h"

typedef CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
                   CCompareString<CSymbolMap::CSymbolInfo> > SymbolStorage;
static SymbolStorage* expected;
static unsigned calls;
static bool correct;

template<>
void SymbolStorage::DeallocateUnusedMemory()
{
    ++calls;
    correct = this == expected;
}

int main()
{
    unsigned char bytes[sizeof(CSymbolMap)];
    memset(bytes, 0, sizeof(bytes));
    CSymbolMap& symbols = *reinterpret_cast<CSymbolMap*>(bytes);
    expected = reinterpret_cast<SymbolStorage*>(bytes + 4);
    symbols.DeallocateUnusedMemory();
    if (calls != 1 || !correct) return 1;
    symbols.DeallocateUnusedMemory();
    if (calls != 2 || !correct) return 2;
    puts("SYMBOL_MAP_SHRINK_PASS");
    return 0;
}
