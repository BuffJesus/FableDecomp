#pragma optimize("s",on)
struct Inner {
    char pad[0x91];
    unsigned char flag; // +0x91
};

struct CObstacleNavigatorExternalInterface {
    void* vtbl;      // +0
    Inner* inner;    // +4
};

bool __fastcall IsDead(CObstacleNavigatorExternalInterface* self)
{
    Inner* p = self->inner;
    bool alive = (p != 0) && ((p->flag & 1) == 0);
    return !alive;
}