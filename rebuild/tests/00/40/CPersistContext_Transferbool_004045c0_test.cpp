#include "fable_persist_context.h"

#include <stdio.h>
#include <string.h>

namespace
{
    int g_crcCalls;
    int g_writeCalls;
    int g_binaryInCalls;
    int g_saveTextCalls;
    int g_loadTextCalls;
    const char* g_name;
    const void* g_stream;
    bool g_written;
    CPersistContext_TransferableComponentGenericType* g_object;
    bool g_objectValue;

    CPersistContext& Reset(CPersistContext& context, CPersistContext::EMode mode)
    {
        memset(&context, 0, sizeof(context));
        context.Mode = mode;
        context.PLoadStream = reinterpret_cast<CMemoryDataInputStream*>(0x11112222);
        context.PSaveStream = reinterpret_cast<CDataOutputStream*>(0x33334444);
        g_crcCalls = g_writeCalls = g_binaryInCalls = g_saveTextCalls = g_loadTextCalls = 0;
        g_name = 0;
        g_stream = 0;
        g_written = false;
        g_object = 0;
        g_objectValue = false;
        return context;
    }

    bool ComponentValue(CPersistContext_TransferableComponentGenericType& object, bool*& value)
    {
        CPersistContext_TransferableComponent<bool>* component =
            static_cast<CPersistContext_TransferableComponent<bool>*>(&object);
        value = component->TransVal;
        return *value;
    }
}

void CPersistContext::CheckCRC(const char* name)
{
    ++g_crcCalls;
    g_name = name;
}

void CDataOutputStream::WriteEBOOL(bool value)
{
    ++g_writeCalls;
    g_stream = this;
    g_written = value;
}

void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& stream, bool& value)
{
    ++g_binaryInCalls;
    g_stream = &stream;
    value = true;
}

void CPersistContext::TransferObjectSaveText(
    const char* name,
    CPersistContext_TransferableComponentGenericType& object)
{
    ++g_saveTextCalls;
    g_name = name;
    g_object = &object;
    bool* value;
    g_objectValue = ComponentValue(object, value);
}

void CPersistContext::TransferObjectLoadText(
    const char* name,
    CPersistContext_TransferableComponentGenericType& object)
{
    ++g_loadTextCalls;
    g_name = name;
    g_object = &object;
    bool* value;
    g_objectValue = ComponentValue(object, value);
    *value = true;
}

void CPersistContext_TransferableComponent<bool>::TransferOut(CPersistContext&, CCharString&)
{
}

void CPersistContext_TransferableComponent<bool>::TransferIn(CPersistContext&, CStringParser&)
{
}

int main()
{
    // The real class has no default constructor; Reset() fills every field.
    static double contextStorage[(sizeof(CPersistContext) + 7) / 8];
    CPersistContext& context = *reinterpret_cast<CPersistContext*>(contextStorage);
    bool value;
    const bool fallbackTrue = true;
    const bool fallbackFalse = false;

    Reset(context, CPersistContext::MODE_DEFAULTS);
    value = false;
    context.Transfer("default", value, fallbackTrue);
    if (!(value && g_crcCalls == 1 && strcmp(g_name, "default") == 0))
        return 1;

    Reset(context, CPersistContext::MODE_BINARY_SAVE);
    value = true;
    context.Transfer("binary-out", value, fallbackFalse);
    if (!(g_writeCalls == 1 && g_stream == context.PSaveStream && g_written && value))
        return 2;

    Reset(context, CPersistContext::MODE_BINARY_LOAD);
    value = false;
    context.Transfer("binary-in", value, fallbackFalse);
    if (!(g_binaryInCalls == 1 && g_stream == context.PLoadStream && value))
        return 3;

    Reset(context, CPersistContext::MODE_TEXT_SAVE);
    value = true;
    context.Transfer("text-save", value, fallbackFalse);
    if (!(g_saveTextCalls == 1 && strcmp(g_name, "text-save") == 0 && g_objectValue && value))
        return 4;

    Reset(context, CPersistContext::MODE_TEXT_LOAD);
    value = false;
    context.Transfer("text-load", value, fallbackTrue);
    if (!(g_loadTextCalls == 1 && !g_objectValue && value))
        return 5;

    Reset(context, CPersistContext::MODE_TEXT_LOAD);
    context.UseDefaults = true;
    value = false;
    context.Transfer("text-default", value, fallbackTrue);
    if (!(g_loadTextCalls == 1 && g_objectValue && value))
        return 6;

    printf("FABLETLC_PERSIST_TRANSFER_BOOL_BEHAVIOR PASS\n");
    return 0;
}
