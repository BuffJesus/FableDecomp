namespace
{
struct CRefCountBlock
{
    int m_nRefCount;
    void (__thiscall* m_pRelease)(void*);
    void* m_pObject;
};
}

void CEngineLandscapeMap::OpenStaticMap(
    CEngineStaticMapFileBlockManager* pStaticMapFileBlockManager,
    CDataInputStream& inputStream)
{
    m_pStaticMapFileBlockManager = pStaticMapFileBlockManager;

    if (inputStream.m_nAbsoluteReadOffset + 0x14U < 0x80000000) {
        if (inputStream.m_nSrcChunkBytesRemaining < 0x14) {
            inputStream.ReadWithSrcChunkOverflow(reinterpret_cast<uchar*>(&m_staticMapHeader), 0x14);
        } else {
            const std::uint32_t* pHeaderWords =
                reinterpret_cast<const std::uint32_t*>(inputStream.m_pReadCursor);
            m_staticMapHeader.field_00 = pHeaderWords[0];
            m_staticMapHeader.field_04 = pHeaderWords[1];
            m_staticMapHeader.field_08 = pHeaderWords[2];
            m_staticMapHeader.field_0C = pHeaderWords[3];
            m_staticMapHeader.field_10 = pHeaderWords[4];
            inputStream.m_pReadCursor += 0x14;
            inputStream.m_nSrcChunkBytesRemaining += -0x14;
            inputStream.m_nAbsoluteReadOffset += 0x14;
        }
    }

    void* const pBlockManagerObject = pStaticMapFileBlockManager->m_pObject;
    CRefCountBlock* const pNewRefCountBlock = pStaticMapFileBlockManager->m_pRefCountBlock;
    if (pNewRefCountBlock != nullptr) {
        pNewRefCountBlock->m_nRefCount = pNewRefCountBlock->m_nRefCount + 1;
    }

    CRefCountBlock* const pOldRefCountBlock = m_pBlockManagerRefCountBlock;
    if (pOldRefCountBlock != pNewRefCountBlock) {
        if (pOldRefCountBlock != nullptr) {
            pOldRefCountBlock->m_nRefCount = pOldRefCountBlock->m_nRefCount + -1;
            if (pOldRefCountBlock->m_nRefCount == 0) {
                pOldRefCountBlock->m_pRelease(pOldRefCountBlock->m_pObject);
                operator delete(pOldRefCountBlock);
            }
        }

        m_pBlockManagerObject = pBlockManagerObject;
        m_pBlockManagerRefCountBlock = pNewRefCountBlock;

        if (pNewRefCountBlock == nullptr) {
            goto after_refcount_update;
        }

        pNewRefCountBlock->m_nRefCount = pNewRefCountBlock->m_nRefCount + 1;
    }

    if ((pNewRefCountBlock != nullptr) &&
        ((pNewRefCountBlock->m_nRefCount = pNewRefCountBlock->m_nRefCount + -1),
         (pNewRefCountBlock->m_nRefCount == 0))) {
        pNewRefCountBlock->m_pRelease(pNewRefCountBlock->m_pObject);
        operator delete(pNewRefCountBlock);
    }

after_refcount_update:
    std::uint32_t allocationSize = m_staticMapHeader.field_04;
    void* pScratchBuffer;
    if (allocationSize == 0) {
        pScratchBuffer = nullptr;
    } else {
        pScratchBuffer = std::malloc(allocationSize);
    }

    void* pZeroCursor = pScratchBuffer;
    for (std::uint32_t wordCount = allocationSize >> 2; wordCount != 0; wordCount = wordCount - 1) {
        *static_cast<std::uint32_t*>(pZeroCursor) = 0;
        pZeroCursor = static_cast<void*>(static_cast<std::uint32_t*>(pZeroCursor) + 1);
    }
    for (std::uint32_t byteCount = allocationSize & 3; byteCount != 0; byteCount = byteCount - 1) {
        *static_cast<std::uint8_t*>(pZeroCursor) = 0;
        pZeroCursor = static_cast<void*>(static_cast<std::uint8_t*>(pZeroCursor) + 1);
    }

    inputStream.SetSourceChunk(m_staticMapHeader.field_00);
    m_texturePalette.Load(inputStream);

    if (pScratchBuffer != nullptr) {
        std::free(pScratchBuffer);
    }
}