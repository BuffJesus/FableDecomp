// CMovableResourceMemoryPool::CanMoveResource  @ 00a62f70
// Static forwarder: (obj, arg). If obj null -> false; else obj->vslot2(arg).

struct IMovableResource {
    virtual void v0();
    virtual void v1();
    virtual bool CanMove(long arg);   // vtable slot 2 -> [vptr+8]
};

bool __stdcall CanMoveResource_forward(IMovableResource *obj, long arg)
{
    if (obj)
        return obj->CanMove(arg);
    return false;
}