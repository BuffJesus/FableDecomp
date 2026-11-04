// CAFile::CopyFileW @ 0x00998E70 (FableWin ?CopyFileW@CAFile@@SIXAAV1@0@Z; the
// source spelled CopyFile and <windows.h> renamed it).
//
// Sizes the destination to the source, rewinds both, then streams the data
// through a zeroed 4 KB buffer (the engine's 12-byte malloc-backed array,
// freed on scope exit), pumping the progress display after every chunk.

#include "fable_memory_file.h"
#include "fable_min_max.h"
#include "fable_progress_display.h"

void CAFile::CopyFileW(CAFile& source, CAFile& destination)
{
    destination.SetLength(source.GetLength());
    destination.SetPosition(0);
    source.SetPosition(0);

    long bufferSize = 0x1000;
    CMemoryFileBuffer<unsigned char> buffer(bufferSize);

    long remaining = source.GetLength();
    while (remaining > 0)
    {
        long chunk = Minimum(remaining, bufferSize);
        source.Read(buffer.First, chunk, 0);
        destination.Write(buffer.First, chunk, 0);
        remaining -= chunk;
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    }
}