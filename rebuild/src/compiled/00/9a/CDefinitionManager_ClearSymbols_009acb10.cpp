#include "fable_definition_manager.h"
#include "fable_symbol_map.h"

void CDefinitionManager::ClearSymbols()
{
    PSymbolMap.Data->Clear();
}
