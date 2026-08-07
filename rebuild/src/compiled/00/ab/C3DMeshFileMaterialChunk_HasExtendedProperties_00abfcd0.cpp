#include "rebuild_abi.h"
// C3DMeshFileMaterialChunk::HasExtendedProperties @ 0x00abfcd0
// push STR; call helper; xor ecx,ecx; cmp ecx,eax; sbb eax,eax; neg eax; ret
// Return whether a named property lookup returned a non-null result.
int FABLE_STDCALL FableHasExtHelper_abfcd0(const char *name);
extern const char kExtPropName_abfcd0[];
struct C3DMeshFileMaterialChunk { bool HasExtendedProperties(); };
bool C3DMeshFileMaterialChunk::HasExtendedProperties()
{
    return (unsigned int)FableHasExtHelper_abfcd0(kExtPropName_abfcd0) > 0;
}