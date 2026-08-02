#include <stdio.h>
#include <string.h>

struct CThing;

class CAIStateGroup_MinionSitOnSeat
{
public:
    bool GetPOwnerObject(CThing** out);
};

bool __fastcall CAIStateGroup_MinionSitOnSeat_GetPOwnerObject(CAIStateGroup_MinionSitOnSeat* self, void* unused_edx, CThing** out)
{
    *out = 0;
    return true;
}

int main()
{
    CAIStateGroup_MinionSitOnSeat obj;
    CThing* sentinel = (CThing*)0x12345678;
    CThing* outPtr = sentinel;
    bool result = CAIStateGroup_MinionSitOnSeat_GetPOwnerObject(&obj, 0, &outPtr);

    if (result != true) {
        printf("FAIL_RESULT\n");
        return 1;
    }
    if (outPtr != 0) {
        printf("FAIL_NOTNULL\n");
        return 1;
    }

    printf("MINIONSIT_GETPOWNER_PASS\n");
    return 0;
}