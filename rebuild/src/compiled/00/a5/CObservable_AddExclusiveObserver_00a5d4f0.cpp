#include "rebuild_abi.h"
// CObservable::AddExclusiveObserver @ 0x00a5d4f0
// mov eax,[esp+4]; mov [ecx+0x38],eax; ret 4  -> this->f38 = observer
struct CObservable { char pad00[0x38]; void *m_observer; void AddExclusiveObserver(void *obs); };
void CObservable::AddExclusiveObserver(void *obs){ m_observer = obs; }