#include "fable_persist_context.h"

#include <new>
#include <stdio.h>
#include <string.h>

namespace
{
    void __fastcall ErrorCallback(const CCharString&)
    {
    }
}

int main()
{
    unsigned char storage[sizeof(CPersistContext)];
    memset(storage, 0xA5, sizeof(storage));

    CStringParser& parser = *reinterpret_cast<CStringParser*>(0x11111111);
    const CDefinitionManager* definitionManager =
        reinterpret_cast<const CDefinitionManager*>(0x33333333);
    const CSymbolMap* symbolMap = reinterpret_cast<const CSymbolMap*>(0x44444444);
    CCharString* stringToClean = reinterpret_cast<CCharString*>(0x55555555);

    CPersistContext* context = new (storage) CPersistContext(
        parser,
        &ErrorCallback,
        true,
        definitionManager,
        symbolMap,
        stringToClean);

    if (sizeof(CPersistContext) != 0x2C || context != reinterpret_cast<CPersistContext*>(storage))
    {
        return 1;
    }
    if (
        context->PDefinitionManager != definitionManager ||
        context->PSymbolMap != symbolMap ||
        context->PParser != &parser ||
        context->ErrorCallback != &ErrorCallback ||
        context->PStringToClean != stringToClean)
    {
        return 2;
    }
    if (
        context->UseDefaults != true ||
        context->UseCRCCheck != false ||
        context->SingleTransfer != false ||
        context->Finished != false ||
        context->Mode != CPersistContext::MODE_TEXT_LOAD)
    {
        return 3;
    }
    // The text-load constructor leaves the binary/save fields untouched.
    for (size_t offset = 0x1C; offset < sizeof(storage); ++offset)
    {
        if (storage[offset] != 0xA5)
        {
            return 4;
        }
    }

    printf("FABLETLC_PERSIST_CONTEXT_TEXT_CTOR_009BADD0 PASS\n");
    return 0;
}
