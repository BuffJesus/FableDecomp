struct RefCounted { int vt; int refcount; };

struct CAnimationEntryData {
    int a;
    int b;
    int c;
    RefCounted* p;
};

CAnimationEntryData* __fastcall CAnimationEntry(CAnimationEntryData* self, void* edx_, CAnimationEntryData* src)
{
    self->a = src->a;
    self->b = src->b;
    self->c = src->c;
    RefCounted* p = src->p;
    self->p = p;
    if (p)
        p->refcount++;
    return self;
}