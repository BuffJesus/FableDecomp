#include "rebuild_abi.h"
#include <cstdio>

struct CEngineSubPrimitiveText;
struct CEngineSubPrimitiveText_Member;

static int g_member_dtor = 0;
static int g_main_dtor = 0;
static int g_deleted = 0;
static void* g_freed = 0;

void FABLE_FASTCALL CEngineSubPrimitiveText_Member_dtor(
        CEngineSubPrimitiveText_Member*) { ++g_member_dtor; }
void FABLE_FASTCALL CEngineSubPrimitiveText_dtor(
        CEngineSubPrimitiveText*) { ++g_main_dtor; }
void FABLE_CDECL fable_operator_delete(void* p) { ++g_deleted; g_freed = p; }

void* FABLE_FASTCALL CEngineSubPrimitiveText_scalar_deleting_dtor(
        CEngineSubPrimitiveText* thisptr, int edx, unsigned int flags);

int main()
{
    // fake object big enough to hold the +0x3c member reference
    fable_u8 buf[0x80];
    CEngineSubPrimitiveText* obj = (CEngineSubPrimitiveText*)buf;

    // flags & 1 == 0 : destruct but do NOT free
    void* r1 = CEngineSubPrimitiveText_scalar_deleting_dtor(obj, 0, 0);

    // flags & 1 == 1 : destruct AND free
    void* r2 = CEngineSubPrimitiveText_scalar_deleting_dtor(obj, 0, 1);

    if (r1 == (void*)obj && r2 == (void*)obj &&
        g_member_dtor == 2 && g_main_dtor == 2 &&
        g_deleted == 1 && g_freed == (void*)obj)
    {
        std::printf("OK_SDD_0047200d\n");
    }
    else
    {
        std::printf("FAIL_0047200d\n");
    }
    return 0;
}