// CMarkerInfo::IsHeroMarker  @ 0082a210
// bool __fastcall CMarkerInfo::IsHeroMarker(CMarkerInfo* this)

struct SomeGlobalHolder {
    char pad[0x958];
    int* pHeroId;   // at +0x958
};

// call 0x438570 -> returns SomeGlobalHolder*
SomeGlobalHolder* GetGlobalHolder(void);

struct CMarkerInfo {
    int field0;   // +0
    int field4;   // +4
    int markerId; // +8
    bool IsHeroMarker();
};

bool CMarkerInfo::IsHeroMarker()
{
    int* p = GetGlobalHolder()->pHeroId;
    return this->markerId == *p;
}