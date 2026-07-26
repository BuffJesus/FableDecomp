struct CTemp { int x; };
extern "C" int __fastcall CTemp_ctor(CTemp* self);
extern "C" void __fastcall CTemp_dtor(CTemp* self);
struct CSpaceChecker { char pad; };
extern CSpaceChecker g_spaceChecker;
extern bool __fastcall CSpaceChecker_Check(CSpaceChecker* self, int arg);

bool __fastcall CUserProfileManager_IsEnoughSpaceForProfile(void* self)
{
    CTemp tmp;
    int r = CTemp_ctor(&tmp);
    bool ok = CSpaceChecker_Check(&g_spaceChecker, r);
    CTemp_dtor(&tmp);
    return ok;
}