#include "fable_persist_context.h"

// CPersistContext::CPersistContext(CStringParser&, void (*)(const CCharString&),
//     bool, const CDefinitionManager*, const CSymbolMap*, CCharString*) @ 0x009BADD0.
//
// The text-load context GFMain Phase 3 builds over the settings parser. Only
// the mode is in the initialiser list; the zero it needs is what keeps every
// argument copy in EDX in retail.
CPersistContext::CPersistContext(
    CStringParser& parser,
    void (__fastcall* errorCallback)(const CCharString&),
    bool useDefaults,
    const CDefinitionManager* definitionManager,
    const CSymbolMap* symbolMap,
    CCharString* stringToClean)
    : Mode(MODE_TEXT_LOAD)
{
    PDefinitionManager = definitionManager;
    PSymbolMap = symbolMap;
    PParser = &parser;
    ErrorCallback = errorCallback;
    PStringToClean = stringToClean;
    UseDefaults = useDefaults;
    UseCRCCheck = false;
    Finished = false;
    SingleTransfer = false;
}
