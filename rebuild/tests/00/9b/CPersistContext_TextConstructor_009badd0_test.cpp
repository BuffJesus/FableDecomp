#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct FablePersistContext_009BADD0
{
    void* definitionManager;
    void* symbolMap;
    void* parser;
    void* errorCallback;
    void* sourceName;
    unsigned char reportErrors;
    unsigned char field15;
    unsigned char field16;
    unsigned char field17;
    unsigned long mode;
};

extern "C" FablePersistContext_009BADD0* FABLE_FASTCALL
FablePersistContextTextConstruct_009BADD0(
    FablePersistContext_009BADD0* self,
    void*,
    void* parser,
    void* errorCallback,
    bool reportErrors,
    void* definitionManager,
    void* symbolMap,
    void* sourceName);

int main()
{
    FablePersistContext_009BADD0 context;
    memset(&context, 0xA5, sizeof(context));

    void* parser = reinterpret_cast<void*>(0x11111111);
    void* callback = reinterpret_cast<void*>(0x22222222);
    void* definitionManager = reinterpret_cast<void*>(0x33333333);
    void* symbolMap = reinterpret_cast<void*>(0x44444444);
    void* sourceName = reinterpret_cast<void*>(0x55555555);

    FablePersistContext_009BADD0* result =
        FablePersistContextTextConstruct_009BADD0(
            &context,
            0,
            parser,
            callback,
            true,
            definitionManager,
            symbolMap,
            sourceName);

    if (result != &context)
    {
        return 1;
    }
    if (
        context.definitionManager != definitionManager ||
        context.symbolMap != symbolMap ||
        context.parser != parser ||
        context.errorCallback != callback ||
        context.sourceName != sourceName)
    {
        return 2;
    }
    if (
        context.reportErrors != 1 ||
        context.field15 != 0 ||
        context.field16 != 0 ||
        context.field17 != 0 ||
        context.mode != 0)
    {
        return 3;
    }

    printf("FABLETLC_PERSIST_CONTEXT_TEXT_CTOR_009BADD0 PASS\n");
    return 0;
}
