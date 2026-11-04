// CDefinitionManager::CDefinitionManager(CWideString const&, void (*)(CCharString const&)) @ 0x009B0470.
#include "fable_definition_manager.h"

// ?CompiledDefDirectory@CDefinitionManager@@1VCWideString@@A (retail
// 0x013CA7DC), defined in this unit (DefinitionManager.cpp): the directory
// compiled .bin files live in. Empty at startup (this unit's dynamic
// initializer, retail 0x01224050, plus its atexit destructor thunk
// 0x0122A6B0); SetCompiledDefDirectory 0x004036B0 fills it in during boot.
CWideString CDefinitionManager::CompiledDefDirectory;

// A definition compiler for the given .bin (under the compiled-def directory):
// empty symbol tables and def maps, a symbol map that already knows NULL = 0,
// and resources that unload as soon as they go unused.
CDefinitionManager::CDefinitionManager(const CWideString& filename, ErrorCallbackFunction errorCallback)
    : ErrorCallback(errorCallback),
      CompiledFileName(CompiledDefDirectory + filename)
{
    UseSafeBinaryFormat = false;
    PDefinitionSymbolMap.Reset(new CSymbolMap);
    PSymbolMap.Reset(new CSymbolMap);
    PSymbolMap->AddSymbolDirty("NULL", 0);
    PSymbolMap->UpdateDirty();
    SetResourceUnloadDelay(0);
}
