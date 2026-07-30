struct Ref { void* pad0; int refcount; };
struct BlendState { void* vtbl; Ref* ref; };
extern void* const C3DAnimationBlendState_vtable;
struct BlendState* __fastcall C3DAnimationBlendState(struct BlendState* self, int edx_ignored, struct BlendState* other)
{
    self->vtbl = (void*)&C3DAnimationBlendState_vtable;
    Ref* r = other->ref;
    self->ref = r;
    if (r)
        r->refcount++;
    return self;
}