#include "fable_checksum_cache_file.h"
CChecksumCacheFile::CChecksumCacheFile(const CWideString& filename,long requestedBufferSize)
 : BufferSize((requestedBufferSize+8)&~8),NeedsSaving(false) {
 Open(filename);
}
