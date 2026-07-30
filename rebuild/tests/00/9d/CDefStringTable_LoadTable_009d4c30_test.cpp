#include <stdio.h>
#include <string.h>

#include "fable_definition_table.h"

extern "C" fable_u8 g_FableDiskFileVTable_0122D06C = 0;

namespace
{
    enum Event
    {
        kBaseConstruct,
        kCharConstruct,
        kOpen,
        kStreamConstruct,
        kRead,
        kStreamDestroy,
        kClose,
        kCharDestroy,
        kBaseDestroy
    };

    Event g_events[9];
    unsigned int g_eventCount;
    bool g_pathExists;
    CDefStringTable* g_expectedTable;
    void* g_file;
    void* g_stream;
    bool g_argumentsValid;
    bool g_flagsVisibleAtDestruction;

    void Record(Event event)
    {
        if (g_eventCount < 9)
        {
            g_events[g_eventCount] = event;
        }
        ++g_eventCount;
    }

    bool CheckMissingPath()
    {
        fable_u8 tableBytes[sizeof(CDefStringTable)];
        memset(tableBytes, 0xA5, sizeof(tableBytes));
        CDefStringTable& table =
            *reinterpret_cast<CDefStringTable*>(tableBytes);
        table.loaded_ = false;
        table.readOnly_ = true;
        g_pathExists = false;
        g_eventCount = 0;

        table.LoadTable(false);
        return
            g_eventCount == 0 &&
            !table.loaded_ &&
            table.readOnly_;
    }

    bool CheckLoadedPath(bool readOnly)
    {
        fable_u8 tableBytes[sizeof(CDefStringTable)];
        memset(tableBytes, 0, sizeof(tableBytes));
        CDefStringTable& table =
            *reinterpret_cast<CDefStringTable*>(tableBytes);
        g_expectedTable = &table;
        g_pathExists = true;
        g_eventCount = 0;
        g_file = 0;
        g_stream = 0;
        g_argumentsValid = true;
        g_flagsVisibleAtDestruction = false;

        table.LoadTable(readOnly);

        static const Event expected[9] =
        {
            kBaseConstruct,
            kCharConstruct,
            kOpen,
            kStreamConstruct,
            kRead,
            kStreamDestroy,
            kClose,
            kCharDestroy,
            kBaseDestroy
        };
        return
            g_eventCount == 9 &&
            memcmp(g_events, expected, sizeof(expected)) == 0 &&
            g_argumentsValid &&
            g_flagsVisibleAtDestruction &&
            table.loaded_ &&
            table.readOnly_ == readOnly;
    }
}

extern "C" bool FABLE_FASTCALL
FableDefTablePathExists_009D4C30(
    void* pathname,
    void*)
{
    if (
        g_expectedTable != 0 &&
        pathname !=
            reinterpret_cast<fable_u8*>(g_expectedTable) + 0x30)
    {
        g_argumentsValid = false;
    }
    return g_pathExists;
}

extern "C" void FABLE_FASTCALL
FableDefTableFileBaseConstruct_009D4C30(
    void* file,
    void*)
{
    Record(kBaseConstruct);
    g_file = file;
}

extern "C" void FABLE_FASTCALL
FableDefTableCharStringConstruct_009D4C30(
    void*,
    void*)
{
    Record(kCharConstruct);
}

extern "C" void FABLE_FASTCALL
FableDefTableDiskFileOpen_009D4C30(
    void* file,
    void*,
    const void* pathname,
    fable_u32 access,
    fable_u32 flags)
{
    Record(kOpen);
    g_argumentsValid =
        g_argumentsValid &&
        file == g_file &&
        pathname ==
            reinterpret_cast<fable_u8*>(g_expectedTable) + 0x30 &&
        access == 1 &&
        flags == 4;
}

extern "C" void FABLE_FASTCALL
FableDefTableInputStreamConstruct_009D4C30(
    void* stream,
    void*,
    void* file,
    fable_u32 bufferSize)
{
    Record(kStreamConstruct);
    g_stream = stream;
    g_argumentsValid =
        g_argumentsValid &&
        file == g_file &&
        bufferSize == 0x4000;
}

extern "C" void FABLE_FASTCALL
FableDefTableRead_009D4C30(
    void* table,
    void*,
    void* stream)
{
    Record(kRead);
    g_argumentsValid =
        g_argumentsValid &&
        table == g_expectedTable &&
        stream == g_stream;
}

extern "C" void FABLE_FASTCALL
FableDefTableInputStreamDestroy_009D4C30(
    void* stream,
    void*)
{
    Record(kStreamDestroy);
    g_argumentsValid =
        g_argumentsValid &&
        stream == g_stream;
    g_flagsVisibleAtDestruction =
        g_expectedTable->loaded_;
}

extern "C" void FABLE_FASTCALL
FableDefTableDiskFileClose_009D4C30(
    void* file,
    void*)
{
    Record(kClose);
    g_argumentsValid =
        g_argumentsValid &&
        file == g_file;
}

extern "C" void FABLE_FASTCALL
FableDefTableCharStringDestroy_009D4C30(
    void*,
    void*)
{
    Record(kCharDestroy);
}

extern "C" void FABLE_FASTCALL
FableDefTableFileBaseDestroy_009D4C30(
    void* file,
    void*)
{
    Record(kBaseDestroy);
    g_argumentsValid =
        g_argumentsValid &&
        file == g_file;
}

int main()
{
    g_expectedTable = 0;
    const bool passed =
        CheckMissingPath() &&
        CheckLoadedPath(false) &&
        CheckLoadedPath(true);
    if (!passed)
    {
        printf("FABLETLC_DEF_STRING_TABLE_LOAD_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_DEF_STRING_TABLE_LOAD_BEHAVIOR PASS\n");
    return 0;
}
