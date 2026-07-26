#include <cstdio>
struct CTemp { int x; };
static int g_ctorCalls = 0;
static int g_dtorCalls = 0;
int CTemp_ctor(CTemp* self){ g_ctorCalls++; self->x = 42; return self->x; }
void CTemp_dtor(CTemp* self){ g_dtorCalls++; (void)self; }
struct CSpaceChecker { char pad; };
static CSpaceChecker g_spaceChecker;
static int g_lastArg = 0;
bool CSpaceChecker_Check(CSpaceChecker* self, int arg){ (void)self; g_lastArg = arg; return arg > 0; }

bool CUserProfileManager_IsEnoughSpaceForProfile(void* self)
{
    (void)self;
    CTemp tmp;
    int r = CTemp_ctor(&tmp);
    bool ok = CSpaceChecker_Check(&g_spaceChecker, r);
    CTemp_dtor(&tmp);
    return ok;
}

int main(){
    bool r = CUserProfileManager_IsEnoughSpaceForProfile((void*)0);
    if (r && g_ctorCalls==1 && g_dtorCalls==1 && g_lastArg==42) {
        std::printf("CUserProfileManager_004067c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d ctor=%d dtor=%d arg=%d\n", (int)r, g_ctorCalls, g_dtorCalls, g_lastArg);
    return 1;
}