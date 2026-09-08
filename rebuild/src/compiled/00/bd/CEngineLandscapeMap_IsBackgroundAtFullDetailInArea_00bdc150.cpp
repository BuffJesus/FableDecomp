// Retail 0x00BDC150. With no background map, every area is already at
// full detail; otherwise delegate the four-coordinate query to that map.
struct CLandscapeBackgroundMap {
    bool IsAtFullDetailInArea(long left, long top, long right, long bottom);
};
struct CEngineLandscapeMap {
    void* __vftable;
    CLandscapeBackgroundMap* m_pBackgroundMap;
    bool IsBackgroundAtFullDetailInArea(long left, long top, long right, long bottom);
};
bool CEngineLandscapeMap::IsBackgroundAtFullDetailInArea(
    long left, long top, long right, long bottom) {
    if (m_pBackgroundMap == 0) return true;
    return m_pBackgroundMap->IsAtFullDetailInArea(left, top, right, bottom);
}