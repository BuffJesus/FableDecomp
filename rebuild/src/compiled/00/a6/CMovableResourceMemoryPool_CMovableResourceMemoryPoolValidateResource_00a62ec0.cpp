#include "engine/CMovableResourceMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
struct IValidatable
{
    void* vtbl[4];
};

typedef void (__fastcall *ValidateFn)(void* self);

struct CMovableResourceMemoryPool_Methods : CMovableResourceMemoryPool {
    void ValidateResource(void* p);
};

void CMovableResourceMemoryPool_Methods::ValidateResource(void* p)
{
    ValidateFn fn = (ValidateFn)(*(void***)p)[3];
    fn(p);
}