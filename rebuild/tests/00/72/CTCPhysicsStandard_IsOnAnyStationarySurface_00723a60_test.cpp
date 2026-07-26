#include <cstdio>
struct Surface { char pad[0x3c]; unsigned char flags; };
struct PhysObj { char pad0[0x60]; Surface* surface; };
struct Owner;
struct OwnerVtbl { char pad0[0x98]; PhysObj* (__fastcall* getPhysObj)(Owner*); };
struct Owner { OwnerVtbl* vt; };
struct SurfVtbl { char pad0[0xd0]; bool (__fastcall* isStationary)(Surface*); };
struct PhysStd;
struct PhysStdVtbl { char pad0[0xdc]; bool (__fastcall* v_dc)(PhysStd*); bool (__fastcall* v_e0)(PhysStd*); };
struct PhysStd { PhysStdVtbl* vt; Owner* owner; };

static bool g_e0, g_dc, g_stat;
bool __fastcall e0_impl(PhysStd*){return g_e0;}
bool __fastcall dc_impl(PhysStd*){return g_dc;}
bool __fastcall stat_impl(Surface*){return g_stat;}

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

static PhysObj* g_po_ptr;
static PhysObj* __fastcall getpo(Owner*){return g_po_ptr;}

int main(){
    static char surfvt[0xd8];
    *(void**)(surfvt+0xd0) = (void*)stat_impl;
    static char surfmem[0x40];
    *(void**)surfmem = surfvt;
    Surface* sp = (Surface*)surfmem;

    static PhysObj po;
    po.surface = sp;
    g_po_ptr = &po;

    static char physvt[0xe8];
    *(void**)(physvt+0xdc) = (void*)dc_impl;
    *(void**)(physvt+0xe0) = (void*)e0_impl;
    static char ownvt[0xa0];
    *(void**)(ownvt+0x98) = (void*)getpo;
    Owner ow; ow.vt = (OwnerVtbl*)ownvt;
    PhysStd ps; ps.vt = (PhysStdVtbl*)physvt; ps.owner = &ow;

    // e0 true -> true
    g_e0=true; g_dc=false; g_stat=false; sp->flags=0;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=true){printf("FAIL1\n");return 1;}
    // e0 false, dc false -> false
    g_e0=false; g_dc=false;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=false){printf("FAIL2\n");return 1;}
    // e0 false, dc true, stationary true -> true
    g_e0=false; g_dc=true; g_stat=true;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=true){printf("FAIL3\n");return 1;}
    // not stationary, flag set -> false
    g_stat=false; sp->flags=1;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=false){printf("FAIL4\n");return 1;}
    // not stationary, flag clear -> true
    sp->flags=0;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=true){printf("FAIL5\n");return 1;}
    // po null -> true
    g_po_ptr=0;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=true){printf("FAIL6\n");return 1;}
    // po ok, surface null -> true
    g_po_ptr=&po; po.surface=0;
    if (CTCPhysicsStandard_IsOnAnyStationarySurface(&ps)!=true){printf("FAIL7\n");return 1;}
    printf("CTCPhysicsStandard_00723a60_TEST PASS\n");
    return 0;
}