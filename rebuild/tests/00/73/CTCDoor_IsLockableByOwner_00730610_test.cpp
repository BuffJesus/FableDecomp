#include "engine/CTCDoor.h"
#include <stdio.h>
struct CDefPointeeBase {
    char pad[0x3a];
    unsigned char lockedFlag;
};
bool __fastcall CTCDoor_IsLockableByOwner(CTCDoor* self) {
    CDefPointeeBase* o = self->PDef_Object;
    if (o != 0)
        return o->lockedFlag == 0;
    return true;
}
int main() {
    CTCDoor d;
    d.PDef_Object = 0;
    bool r1 = CTCDoor_IsLockableByOwner(&d);
    CDefPointeeBase o;
    o.lockedFlag = 0;
    d.PDef_Object = &o;
    bool r2 = CTCDoor_IsLockableByOwner(&d);
    o.lockedFlag = 1;
    bool r3 = CTCDoor_IsLockableByOwner(&d);
    if (r1 == true && r2 == true && r3 == false) {
        printf("OK_00730610\n");
    } else {
        printf("FAIL r1=%d r2=%d r3=%d\n", r1, r2, r3);
    }
    return 0;
}