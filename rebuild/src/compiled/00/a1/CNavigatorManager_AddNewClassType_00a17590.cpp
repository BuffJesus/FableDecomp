// CNavigatorManager::AddNewClassType (retail 0x00A17590): CNavigatorManager
// vtable 0x0129CA84 slot 1. Tail-calls CClassFactory::AddNewClassType
// (0x00A36F90). Ego_r.pdb ?AddNewClassType@CNavigatorManager@@UAEXABVCClassTypeInit@@@Z.
#include "fable_navigator.h"

void CNavigatorManager::AddNewClassType(const CClassTypeInit& init)
{
    CClassFactory::AddNewClassType(init);
}
