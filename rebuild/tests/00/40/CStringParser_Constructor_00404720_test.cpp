#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct FableStringParser_00404720
{
    unsigned char bytes[0x18];
};

extern "C" FableStringParser_00404720* FABLE_FASTCALL
FableStringParserConstruct_00404720(
    FableStringParser_00404720* self,
    void*,
    const void* input,
    void* errorCallback,
    void* sourceName,
    long mode);

namespace
{
    struct TestStringStorage
    {
        const char* text;
    };

    struct TestCharString
    {
        TestStringStorage* storage;
    };

    unsigned char g_listNode[0x10];
    unsigned int g_baseConstructCalls;
    unsigned int g_stringConstructCalls;
    unsigned int g_assignCalls;
    unsigned int g_initializeCalls;
    void* g_assignedDestination;
    const void* g_assignedSource;
    void* g_initializeSelf;
    const char* g_initializeText;
    void* g_initializeCallback;
    void* g_initializeSource;
    long g_initializeMode;
}

extern "C" void FABLE_FASTCALL
FableStringParserBaseConstruct_0099A2D0(void* self, void*)
{
    ++g_baseConstructCalls;
    if (self)
    {
        memset(self, 0xCC, 0x0C);
    }
}

extern "C" void FABLE_FASTCALL
FableCharStringConstruct_0099E4B0(void* self, void*)
{
    ++g_stringConstructCalls;
    *static_cast<void**>(self) = 0;
}

extern "C" void* FABLE_CDECL
FableAllocate_00BFEA0E(unsigned long size)
{
    if (size != sizeof(g_listNode))
    {
        return 0;
    }
    memset(g_listNode, 0xCD, sizeof(g_listNode));
    return g_listNode;
}

extern "C" void* FABLE_FASTCALL
FableCharStringAssign_0099EFB0(
    void* self,
    void*,
    const void* source)
{
    ++g_assignCalls;
    g_assignedDestination = self;
    g_assignedSource = source;
    *static_cast<void**>(self) =
        *static_cast<void* const*>(source);
    return self;
}

extern "C" void FABLE_FASTCALL
FableStringParserInitialize_009B9530(
    void* self,
    void*,
    const char* text,
    void* errorCallback,
    void* sourceName,
    long mode)
{
    ++g_initializeCalls;
    g_initializeSelf = self;
    g_initializeText = text;
    g_initializeCallback = errorCallback;
    g_initializeSource = sourceName;
    g_initializeMode = mode;
}

int main()
{
    TestStringStorage storage = {"Language English;"};
    TestCharString input = {&storage};
    int callbackToken;
    int sourceToken;
    FableStringParser_00404720 parser;
    memset(&parser, 0xA5, sizeof(parser));

    FableStringParser_00404720* result =
        FableStringParserConstruct_00404720(
            &parser,
            0,
            &input,
            &callbackToken,
            &sourceToken,
            7);

    if (result != &parser)
    {
        return 1;
    }
    if (
        g_baseConstructCalls != 1 ||
        g_stringConstructCalls != 2 ||
        g_assignCalls != 1 ||
        g_initializeCalls != 1)
    {
        return 2;
    }
    if (
        *reinterpret_cast<unsigned long*>(&parser.bytes[0]) !=
            0x0122D874ul ||
        g_assignedDestination != &parser.bytes[0x10] ||
        g_assignedSource != &input)
    {
        return 3;
    }
    if (
        *reinterpret_cast<void**>(&parser.bytes[0x14]) != g_listNode ||
        *reinterpret_cast<void**>(&g_listNode[0]) != g_listNode ||
        *reinterpret_cast<void**>(&g_listNode[4]) != g_listNode)
    {
        return 4;
    }
    if (
        g_initializeSelf != &parser ||
        g_initializeText != storage.text ||
        g_initializeCallback != &callbackToken ||
        g_initializeSource != &sourceToken ||
        g_initializeMode != 7)
    {
        return 5;
    }

    printf("FABLETLC_STRING_PARSER_CTOR_00404720 PASS\n");
    return 0;
}
