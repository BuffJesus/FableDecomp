struct Surface { char pad[0x3c]; unsigned char flags; };
struct PhysObj { char pad0[0x60]; Surface* surface; };
struct Owner;
struct OwnerVtbl { char pad0[0x98]; PhysObj* (__fastcall* getPhysObj)(Owner*); };
struct Owner { OwnerVtbl* vt; };
struct SurfVtbl { char pad0[0xd0]; bool (__fastcall* isStationary)(Surface*); };
struct PhysStd;
struct PhysStdVtbl { char pad0[0xdc]; bool (__fastcall* v_dc)(PhysStd*); bool (__fastcall* v_e0)(PhysStd*); };
struct PhysStd { PhysStdVtbl* vt; Owner* owner; };

bool __fastcall CTCPhysicsStandard_IsOnAnyStationarySurface(PhysStd* self)
{
    if (self->vt->v_e0(self))
        return true;
    if (self->vt->v_dc(self)) {
        PhysObj* po = self->owner->vt->getPhysObj(self->owner);
        if (po) {
            Surface* s = po->surface;
            if (s) {
                if (((SurfVtbl*)*(void**)s)->isStationary(s))
                    return true;
                if (po->surface->flags & 1)
                    return false;
            }
        }
        return true;
    }
    return false;
}