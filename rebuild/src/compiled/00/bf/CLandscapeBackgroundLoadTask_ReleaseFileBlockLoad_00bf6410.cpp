struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(long fileBlockPosition);
};

struct CLandscapeBackgroundLoadTask {
    void* LoadingTree;
    CEngineStaticMapFileBlockManager* FileBlockManager;
    long FileBlockPos;
    unsigned char LOD;
    bool IsPatchLoadTask;
    bool BlockLoading;

    void ReleaseFileBlockLoad();
};

void CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad() {
    if (BlockLoading) {
        FileBlockManager->ReleaseRequest(FileBlockPos);
        BlockLoading = false;
    }
}