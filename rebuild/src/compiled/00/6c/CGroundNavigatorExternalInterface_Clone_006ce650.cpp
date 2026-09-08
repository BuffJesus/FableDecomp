#include "engine/CGroundNavigatorExternalInterface.h"

struct CNavigatorExternalInterface;
extern "C" void* __cdecl op_new(unsigned int size);

struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    CNavigatorExternalInterface* Construct(CGroundNavigatorExternalInterface* source);
    CNavigatorExternalInterface* Clone();
};

CNavigatorExternalInterface* CGroundNavigatorExternalInterface_Methods::Clone()
{
    void* memory = op_new(0xc);
    if (memory) {
        return ((CGroundNavigatorExternalInterface_Methods*)memory)->Construct(this);
    }
    return 0;
}
