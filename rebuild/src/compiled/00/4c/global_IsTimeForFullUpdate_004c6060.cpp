// CBulletTimeManager::IsTimeForFullUpdate @ 0x004c6060
// __fastcall bool(CBulletTimeManager*)

extern "C" int __fastcall sub_4c5fa0(void* self);      // this->method, result unused
extern "C" int __cdecl   sub_bfea70(void);             // returns divisor -> esi
extern "C" int __cdecl   sub_49d870(void);             // returns dividend -> eax

struct CBulletTimeManager;

extern "C" bool __fastcall IsTimeForFullUpdate(CBulletTimeManager* self)
{
    sub_4c5fa0(self);
    int divisor = sub_bfea70();
    int dividend = sub_49d870();
    return (dividend % divisor) == 0;
}