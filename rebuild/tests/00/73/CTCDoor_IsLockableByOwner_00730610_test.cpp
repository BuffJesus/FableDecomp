#include <stdio.h>
struct COwner {
    char pad[0x3a];
    unsigned char lockedFlag;
};
struct CTCDoor {
    char pad[0x20];
    COwner* owner;
};
bool __fastcall CTCDoor_IsLockableByOwner(CTCDoor* self) {
    COwner* o = self->owner;
    if (o != 0)
        return o->lockedFlag == 0;
    return true;
}
int main() {
    CTCDoor d;
    d.owner = 0;
    bool r1 = CTCDoor_IsLockableByOwner(&d);
    COwner o;
    o.lockedFlag = 0;
    d.owner = &o;
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