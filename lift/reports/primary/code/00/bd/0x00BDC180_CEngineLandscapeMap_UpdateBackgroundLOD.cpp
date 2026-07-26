LS_LOADING_STATUS CEngineLandscapeMap::UpdateBackgroundLOD(
    CLandscapeGPUUpdateQueue& gpuUpdateQueue,
    double distanceToCamera,
    bool forceUpdate)
{
    (void)gpuUpdateQueue;

    LS_LOADING_STATUS status = static_cast<LS_LOADING_STATUS>(1);
    m_bLandscapeUpdateActive = true;

    if (m_pBackgroundTree == nullptr)
    {
        if (m_pBackgroundLoadEntry == nullptr)
        {
            if (m_pOwnerMap->m_pEdgeHeightFile == nullptr)
            {
                CLandscapeBackgroundTreeNode* backgroundTree =
                    new CLandscapeBackgroundTreeNode(this, nullptr, m_landscapeMapIndex);

                m_pBackgroundTree = backgroundTree;

                m_pBackgroundTree->BuildDefaultSection(
                    0,
                    0,
                    static_cast<unsigned int>(m_pOwnerMap->m_backgroundWidthU16),   // +0x5C
                    static_cast<unsigned int>(m_pOwnerMap->m_backgroundHeightU16)); // +0x5E
            }
        }
        else
        {
            if (!m_bBackgroundHeaderRequestIssued)
            {
                m_pStaticMapFileBlockManager->AddRequest(
                    m_backgroundHeaderBlockId,
                    m_backgroundHeaderBlockSize,
                    false);
                m_bBackgroundHeaderRequestIssued = true;
            }

            if (m_pStaticMapFileBlockManager->IsBlockLoaded(m_backgroundHeaderBlockId))
            {
                CLandscapeBackgroundTreeNode* backgroundTree =
                    new CLandscapeBackgroundTreeNode(this, nullptr, m_landscapeMapIndex);

                m_pBackgroundTree = backgroundTree;

                if (m_pBackgroundTree != nullptr)
                {
                    const unsigned long blockSize = m_backgroundHeaderBlockSize;
                    unsigned char* blockData =
                        m_pStaticMapFileBlockManager->GetBlockData(m_backgroundHeaderBlockId);

                    CMemoryDataInputStream stream(blockData, blockSize);
                    m_pBackgroundTree->LoadHeader(stream);
                    CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad();
                    // Automatic destruction of `stream` at scope exit maps to Ghidra's
                    // explicit `CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk*)local_1c)`.
                }
            }
            else
            {
                status = static_cast<LS_LOADING_STATUS>(2);
            }
        }
    }

    if (m_pBackgroundTree != nullptr)
    {
        if (!m_bForegroundLODUpdateActive)
        {
            status = m_pBackgroundTree->UpdateStaticLOD(
                distanceToCamera,
                forceUpdate,
                m_pStaticMapFileBlockManager);
        }
    }

    if (m_pStaticMapFileBlockManager != nullptr)
    {
        BankFileList_RemoveFinished();
    }

    m_bLandscapeUpdateActive = false;
    return status;
}