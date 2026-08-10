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