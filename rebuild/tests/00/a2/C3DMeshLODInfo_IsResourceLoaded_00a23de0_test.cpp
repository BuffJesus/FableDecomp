#include <stdio.h>
struct CBankFileAsyncData {
    bool finished;
    bool IsFinished();
};
bool CBankFileAsyncData::IsFinished() { return finished; }

struct C3DMeshLODInfo {
    char m_leading[0x24];
    CBankFileAsyncData* m_pAsyncData;
    bool IsResourceLoaded();
};
bool C3DMeshLODInfo::IsResourceLoaded() {
    if (m_pAsyncData != 0) {
        return m_pAsyncData->IsFinished();
    }
    return true;
}
int main() {
    C3DMeshLODInfo info;
    info.m_pAsyncData = 0;
    if (!info.IsResourceLoaded()) return 1;
    CBankFileAsyncData async;
    async.finished = false;
    info.m_pAsyncData = &async;
    if (info.IsResourceLoaded()) return 2;
    async.finished = true;
    if (!info.IsResourceLoaded()) return 3;
    printf("IS_RESOURCE_LOADED PASS\n");
    return 0;
}