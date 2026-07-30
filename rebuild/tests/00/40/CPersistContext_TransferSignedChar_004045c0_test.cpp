#include <stdio.h>
#include <string.h>

#include "fable_persist_context.h"

extern "C" fable_u8 g_FableSignedCharType_0122D6C4 = 0;
extern "C" void FablePersistSignedCharDefault_004045DB();
extern "C" void FablePersistSignedCharBinaryOut_004045EF();
extern "C" void FablePersistSignedCharBinaryIn_00404608();
extern "C" void FablePersistSignedCharTextSave_00404622();
extern "C" void FablePersistSignedCharTextLoad_00404647();

extern "C" void* g_FableTransferSignedCharJumpTable_0040467C[5] =
{
    reinterpret_cast<void*>(&FablePersistSignedCharTextLoad_00404647),
    reinterpret_cast<void*>(&FablePersistSignedCharTextSave_00404622),
    reinterpret_cast<void*>(&FablePersistSignedCharBinaryIn_00404608),
    reinterpret_cast<void*>(&FablePersistSignedCharBinaryOut_004045EF),
    reinterpret_cast<void*>(&FablePersistSignedCharDefault_004045DB)
};

namespace
{
    struct TextDescriptor
    {
        void* type;
        signed char* value;
    };

    unsigned char g_contextStorage[0x2C];
    fable_u32 g_crcCalls;
    fable_u32 g_writeCalls;
    fable_u32 g_binaryInCalls;
    fable_u32 g_textSaveCalls;
    fable_u32 g_textLoadCalls;
    const char* g_observedName;
    void* g_observedStream;
    fable_u32 g_observedWriteValue;
    void* g_observedType;
    signed char g_observedTextValue;

    fable_u32& Dword(size_t offset)
    {
        return *reinterpret_cast<fable_u32*>(g_contextStorage + offset);
    }

    CPersistContext& Reset(fable_u32 mode)
    {
        memset(g_contextStorage, 0, sizeof(g_contextStorage));
        Dword(0x18) = mode;
        Dword(0x24) = 0x11112222;
        Dword(0x28) = 0x33334444;
        g_crcCalls = 0;
        g_writeCalls = 0;
        g_binaryInCalls = 0;
        g_textSaveCalls = 0;
        g_textLoadCalls = 0;
        g_observedName = 0;
        g_observedStream = 0;
        g_observedWriteValue = 0;
        g_observedType = 0;
        g_observedTextValue = 0;
        return *reinterpret_cast<CPersistContext*>(g_contextStorage);
    }

    bool CheckDefaultMode()
    {
        CPersistContext& context = Reset(4);
        signed char value = 9;
        const signed char fallback = -7;
        context.TransferSignedChar("default", value, fallback);
        return
            value == fallback &&
            g_crcCalls == 1 &&
            strcmp(g_observedName, "default") == 0;
    }

    bool CheckBinaryOutMode()
    {
        CPersistContext& context = Reset(3);
        signed char value = -1;
        const signed char fallback = 0;
        context.TransferSignedChar("binary-out", value, fallback);
        return
            g_writeCalls == 1 &&
            g_observedStream == reinterpret_cast<void*>(0x33334444) &&
            g_observedWriteValue == 255;
    }

    bool CheckBinaryInMode()
    {
        CPersistContext& context = Reset(2);
        signed char value = 0;
        const signed char fallback = 3;
        context.TransferSignedChar("binary-in", value, fallback);
        return
            g_binaryInCalls == 1 &&
            g_observedStream == reinterpret_cast<void*>(0x11112222) &&
            value == 41;
    }

    bool CheckTextSaveMode()
    {
        CPersistContext& context = Reset(1);
        signed char value = -12;
        const signed char fallback = 2;
        context.TransferSignedChar("text-save", value, fallback);
        return
            g_textSaveCalls == 1 &&
            strcmp(g_observedName, "text-save") == 0 &&
            g_observedType == &g_FableSignedCharType_0122D6C4 &&
            g_observedTextValue == -12;
    }

    bool CheckTextLoadWithoutDefault()
    {
        CPersistContext& context = Reset(0);
        signed char value = 17;
        const signed char fallback = -5;
        context.TransferSignedChar("text-load", value, fallback);
        return
            g_textLoadCalls == 1 &&
            g_observedTextValue == 17 &&
            value == 55;
    }

    bool CheckTextLoadWithDefault()
    {
        CPersistContext& context = Reset(0);
        g_contextStorage[0x14] = 1;
        signed char value = 17;
        const signed char fallback = -5;
        context.TransferSignedChar("text-default", value, fallback);
        return
            g_textLoadCalls == 1 &&
            g_observedTextValue == -5 &&
            value == 55;
    }
}

extern "C" void __fastcall FablePersistCheckCrc_004045c0(
    void*,
    void*,
    const char* name)
{
    ++g_crcCalls;
    g_observedName = name;
}

extern "C" void __fastcall FablePersistWriteBool_004045c0(
    void* stream,
    void*,
    fable_u32 value)
{
    ++g_writeCalls;
    g_observedStream = stream;
    g_observedWriteValue = value;
}

extern "C" void __fastcall FablePersistTransferBinaryIn_004045c0(
    void*,
    void*,
    void* stream,
    signed char* value)
{
    ++g_binaryInCalls;
    g_observedStream = stream;
    *value = 41;
}

extern "C" void __fastcall FablePersistTransferObjectSaveText_004045c0(
    void*,
    void*,
    const char* name,
    void* descriptor)
{
    ++g_textSaveCalls;
    g_observedName = name;
    TextDescriptor* text = static_cast<TextDescriptor*>(descriptor);
    g_observedType = text->type;
    g_observedTextValue = *text->value;
}

extern "C" void __fastcall FablePersistTransferObjectLoadText_004045c0(
    void*,
    void*,
    const char* name,
    void* descriptor)
{
    ++g_textLoadCalls;
    g_observedName = name;
    TextDescriptor* text = static_cast<TextDescriptor*>(descriptor);
    g_observedType = text->type;
    g_observedTextValue = *text->value;
    *text->value = 55;
}

int main()
{
    const bool passed =
        CheckDefaultMode() &&
        CheckBinaryOutMode() &&
        CheckBinaryInMode() &&
        CheckTextSaveMode() &&
        CheckTextLoadWithoutDefault() &&
        CheckTextLoadWithDefault();
    if (!passed)
    {
        printf("FABLETLC_PERSIST_SIGNED_CHAR_BEHAVIOR FAIL\n");
        return 1;
    }
    printf("FABLETLC_PERSIST_SIGNED_CHAR_BEHAVIOR PASS\n");
    return 0;
}
