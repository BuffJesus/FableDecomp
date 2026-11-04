// CProfileManager::Update() @ 009f8c10 (1 byte: ret)
//
// Profiling is compiled out of retail, so the body is empty. Identity: the
// retail run 0x009F8BE0..0x009F8D60 follows Ego_r's CProfileManager member
// order one-for-one (DrawProfileHierarchy ret 8, SaveEntry ret 0xC,
// DrawProfileTotals ret 8, Update ret, RegisterProfile xor eax/ret 4,
// Start/StopProfileWithID ret 4, UpdateProfilePercentages ret, CProfile ctor
// 0x009F8C60 ...), and the sole retail caller 0x00418AF4 loads
// CSystemManager+0xF8 (the CCountedPointer<CProfileManager>) into ecx first.
// The manifest's "$E2" is a propagated label.

#include "fable_system_manager.h"

void CProfileManager::Update()
{
}
