#include "rebuild_abi.h"

#include <stdio.h>
#include <wchar.h>

struct FableWideString_00999230
{
    void* storage;
};

extern "C" bool FABLE_FASTCALL
FablePathExists_00999230(
    const FableWideString_00999230& pathname,
    void*);

namespace
{
    struct TestStorage
    {
        const wchar_t* text;
    };

    TestStorage g_input = {L"Data\\lang\\English\\lang_settings.txt"};
    TestStorage g_context = {L"D:\\Fable"};
    TestStorage g_expanded = {
        L"D:\\Fable\\Data\\lang\\English\\lang_settings.txt"};
    unsigned int g_constructCalls;
    unsigned int g_contextCalls;
    unsigned int g_expandCalls;
    unsigned int g_assignCalls;
    unsigned int g_convertCalls;
    unsigned int g_destroyCalls;
    unsigned int g_attributesCalls;
    unsigned long g_attributeResult;

    unsigned long __stdcall GetAttributesStub(const wchar_t* pathname)
    {
        ++g_attributesCalls;
        if (wcscmp(pathname, g_expanded.text) != 0)
        {
            return 0xFFFFFFFFul;
        }
        return g_attributeResult;
    }
}

typedef unsigned long (__stdcall *FableGetFileAttributesWProc)(
    const wchar_t*);
extern "C" FableGetFileAttributesWProc
g_FableGetFileAttributesW_0143FDBC = GetAttributesStub;

extern "C" void FABLE_FASTCALL
FableWideStringConstruct_0099AED0(
    FableWideString_00999230* self,
    void*)
{
    ++g_constructCalls;
    self->storage = 0;
}

extern "C" FableWideString_00999230* FABLE_FASTCALL
FableGetCurrentPath_00997F80(
    FableWideString_00999230* result,
    void*)
{
    ++g_contextCalls;
    result->storage = &g_context;
    return result;
}

extern "C" FableWideString_00999230* FABLE_FASTCALL
FableExpandPath_00998C90(
    FableWideString_00999230* result,
    const FableWideString_00999230* pathname,
    const FableWideString_00999230* context)
{
    ++g_expandCalls;
    if (pathname->storage != &g_input || context->storage != &g_context)
    {
        result->storage = 0;
    }
    else
    {
        result->storage = &g_expanded;
    }
    return result;
}

extern "C" FableWideString_00999230* FABLE_FASTCALL
FableWideStringAssign_0099B7D0(
    FableWideString_00999230* self,
    void*,
    const FableWideString_00999230* other)
{
    ++g_assignCalls;
    self->storage = other->storage;
    return self;
}

extern "C" void FABLE_FASTCALL
FableWideStringDestroy_0099B510(
    FableWideString_00999230* self,
    void*)
{
    ++g_destroyCalls;
    self->storage = 0;
}

extern "C" const wchar_t* FABLE_FASTCALL
FableWideStringGetBuffer_0099AF00(
    FableWideString_00999230* self,
    void*)
{
    ++g_convertCalls;
    TestStorage* storage = static_cast<TestStorage*>(self->storage);
    return storage ? storage->text : L"";
}

int main()
{
    FableWideString_00999230 pathname;
    pathname.storage = &g_input;

    g_attributeResult = 0x20;
    if (!FablePathExists_00999230(pathname, 0))
    {
        return 1;
    }

    g_attributeResult = 0xFFFFFFFFul;
    if (FablePathExists_00999230(pathname, 0))
    {
        return 2;
    }

    if (
        g_constructCalls != 2 ||
        g_contextCalls != 2 ||
        g_expandCalls != 2 ||
        g_assignCalls != 2 ||
        g_convertCalls != 2 ||
        g_destroyCalls != 6 ||
        g_attributesCalls != 2)
    {
        return 3;
    }

    printf("FABLETLC_PATH_EXISTS_00999230 PASS\n");
    return 0;
}
