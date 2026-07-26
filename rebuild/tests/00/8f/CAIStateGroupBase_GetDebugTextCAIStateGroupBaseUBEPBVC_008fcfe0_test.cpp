#include <cstdio>
class CCharString;
struct CAIStateGroupBase { int dummy; };
const CCharString* __fastcall CAIStateGroupBase_GetDebugText(const CAIStateGroupBase* self)
{
  (void)self;
  return 0;
}
int main()
{
  CAIStateGroupBase obj; obj.dummy = 1;
  const CCharString* r = CAIStateGroupBase_GetDebugText(&obj);
  if (r == 0) { std::printf("CAIStateGroupBase_008fcfe0_TEST PASS\n"); return 0; }
  std::printf("FAIL\n"); return 1;
}