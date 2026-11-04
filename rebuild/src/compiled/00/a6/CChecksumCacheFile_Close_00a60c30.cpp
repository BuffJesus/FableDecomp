#include "fable_checksum_cache_file.h"
void CChecksumCacheFile::Close()
{
    if (Filename.GetLength() != 0) {
        Save();
        Entries.clear();
        Filename = L"";
    }
}
