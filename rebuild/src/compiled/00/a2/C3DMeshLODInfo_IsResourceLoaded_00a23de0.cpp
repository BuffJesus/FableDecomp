// Retail 0x00A23DE0. A pending async record answers for itself, tail-called
// (CBankFileAsyncData::IsFinished, 0x009D54E0); no record means already resident.
// The non-null case is tested first so the `return true` block lands last, which is
// the layout retail emitted.
struct CBankFileAsyncData {
    bool IsFinished();
};

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