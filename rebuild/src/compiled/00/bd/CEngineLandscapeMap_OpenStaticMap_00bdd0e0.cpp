#include <malloc.h>
#include <new>
#include "candidates/landscape_map_open_compiled.h"

void CEngineLandscapeMap::OpenStaticMap(
    CEngineStaticMapFileBlockManagerRef* manager,
    CDataInputStream& stream)
{
    StaticMapFileBlockManager = manager;

    if ((unsigned long)(stream.ReadPosition + 0x14) < 0x80000000U)
    {
        if (stream.SrcBytesRemaining < 0x14)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<unsigned char*>(&StaticMapHeader),
                0x14);
        }
        else
        {
            fable_u32* source = stream.SrcCursor32;
            StaticMapHeader.words[0] = source[0];
            StaticMapHeader.words[1] = source[1];
            StaticMapHeader.words[2] = source[2];
            StaticMapHeader.words[3] = source[3];
            StaticMapHeader.words[4] = source[4];
            stream.SrcCursor32 = source + 5;
            stream.SrcBytesRemaining -= 0x14;
            stream.ReadPosition += 0x14;
        }
    }

    void* newObject = manager->Object;
    CRefCountBlock* newRef = manager->RefCountBlock;
    if (newRef != 0)
        ++newRef->RefCount;

    CRefCountBlock* oldRef = BlockManagerRefCountBlock;
    if (oldRef != newRef)
    {
        if (oldRef != 0)
        {
            --oldRef->RefCount;
            if (oldRef->RefCount == 0)
            {
                (static_cast<CReleaseTarget*>(oldRef->Object)->*oldRef->Release)();
                operator delete(oldRef);
            }
        }
        BlockManagerObject = newObject;
        BlockManagerRefCountBlock = newRef;
        if (newRef != 0)
            ++newRef->RefCount;
    }

    if (newRef != 0)
    {
        --newRef->RefCount;
        if (newRef->RefCount == 0)
        {
            (static_cast<CReleaseTarget*>(newRef->Object)->*newRef->Release)();
            operator delete(newRef);
        }
    }

    unsigned long allocationSize = StaticMapHeader.words[1];
    void* scratch = allocationSize == 0 ? 0 : malloc(allocationSize);
    unsigned long* wordCursor = static_cast<unsigned long*>(scratch);
    unsigned long wordCount = allocationSize >> 2;
    while (wordCount != 0)
    {
        *wordCursor++ = 0;
        --wordCount;
    }
    unsigned char* byteCursor = reinterpret_cast<unsigned char*>(wordCursor);
    unsigned long byteCount = allocationSize & 3;
    while (byteCount != 0)
    {
        *byteCursor++ = 0;
        --byteCount;
    }

    stream.SetSrcChunk(StaticMapHeader.words[0]);
    TexturePalette.Load(stream);
    if (scratch != 0)
        free(scratch);
}
