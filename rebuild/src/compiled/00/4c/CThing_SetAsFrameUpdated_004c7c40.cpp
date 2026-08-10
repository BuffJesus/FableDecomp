struct CThing;
struct GlobalMgr {
    void Notify(CThing* t);
};
extern GlobalMgr* g_mgr;
struct CThing {
    unsigned char pad[0x91];
    unsigned char flags;
};
void __fastcall SetAsFrameUpdated(CThing* self) {
    if (!(self->flags & 1))
        g_mgr->Notify(self);
}