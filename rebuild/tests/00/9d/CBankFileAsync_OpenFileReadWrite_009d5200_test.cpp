#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>

static CBankFile* g_file;
static const CWideString* g_name;
static unsigned long g_openFlags;
static unsigned long g_flags;
static bool g_wasDisabled;
static int g_calls;

void CBankFile::OpenFileReadWrite(const CWideString& name,
                                 unsigned long openFlags,
                                 unsigned long flags)
{
    g_file = this;
    g_name = &name;
    g_openFlags = openFlags;
    g_flags = flags;
    g_wasDisabled = static_cast<CBankFileAsync*>(this)->DisableAsync;
    ++g_calls;
}

int main()
{
    unsigned char fileBytes[sizeof(CBankFileAsync)];
    unsigned char nameBytes[sizeof(CWideString)];
    CBankFileAsync* file = reinterpret_cast<CBankFileAsync*>(fileBytes);
    const CWideString* name = reinterpret_cast<const CWideString*>(nameBytes);
    memset(nameBytes, 0x5a, sizeof(nameBytes));
    for (int i = 0; i < 16; ++i) {
        memset(fileBytes, 0xa5, sizeof(fileBytes));
        file->DisableAsync = false;
        g_calls = 0;
        file->CBankFileAsync::OpenFileReadWrite(*name, 100 + i, 200 + i);
        if (g_calls != 1 || g_file != file || g_name != name ||
            g_openFlags != static_cast<unsigned long>(100 + i) ||
            g_flags != static_cast<unsigned long>(200 + i) ||
            g_wasDisabled || !file->DisableAsync) return 1;
        for (unsigned j = 0; j < sizeof(nameBytes); ++j)
            if (nameBytes[j] != 0x5a) return 2;
    }
    puts("BANK_FILE_ASYNC_OPEN_RW_PASS cases=16");
    return 0;
}
