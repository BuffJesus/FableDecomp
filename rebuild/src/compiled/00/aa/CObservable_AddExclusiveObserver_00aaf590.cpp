// CObservable::AddExclusiveObserver  retail 0x00aaf590
// __fastcall void(CObservable*, CObserver*)
// mov eax,[esp+4]; mov [ecx+0x30],eax; ret 4

class CObserver;

class CObservable {
public:
    char pad0[0x30];
    CObserver* m_exclusiveObserver; // +0x30
    void AddExclusiveObserver(CObserver* obs);
};

void CObservable::AddExclusiveObserver(CObserver* obs) {
    m_exclusiveObserver = obs;
}