#include "rebuild_abi.h"

struct CSystemManager;

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    return reinterpret_cast<CSystemManager*>(0x013CA618);
}
