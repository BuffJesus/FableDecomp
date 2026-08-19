// `return (float)fabs(this->value);` — VC7.1 lowers the fabs intrinsic to `fld member; fabs`.
// __fastcall this=ecx, no args.
#include <math.h>
#pragma intrinsic(fabs)
#pragma pack(push,1)
struct T { char pad[0x278]; float value; float GetSeaLevel(); };
#pragma pack(pop)
float T::GetSeaLevel() { return (float)fabs(this->value); }
