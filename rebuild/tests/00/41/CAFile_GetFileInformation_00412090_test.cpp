#include "fable_filesystem.h"
#include "fable_file_information.h"
#include <stdio.h>
#include <string.h>

static CDateAndTime* g_calls[3];
static int g_count;

CDateAndTime::CDateAndTime()
{
    if (g_count < 3) g_calls[g_count] = this;
    ++g_count;
    memset(this, 0, sizeof(*this));
}

int main()
{
    unsigned char fake[sizeof(CAFile)];
    memset(fake, 0xa5, sizeof(fake));
    CAFile* file = reinterpret_cast<CAFile*>(fake);
    for (int i = 0; i < 16; ++i) {
        g_count = 0;
        CFileInformation info = file->CAFile::GetFileInformation();
        if (g_count != 3 ||
            g_calls[0] != &info.CreationTime ||
            g_calls[1] != &info.LastAccessTime ||
            g_calls[2] != &info.LastWriteTime) return 1;
        const unsigned long* words = reinterpret_cast<const unsigned long*>(&info);
        for (int j = 0; j < 21; ++j) if (words[j] != 0) return 2;
        for (unsigned j = 0; j < sizeof(fake); ++j) if (fake[j] != 0xa5) return 3;
    }
    puts("CAFILE_FILE_INFO_PASS cases=16");
    return 0;
}
