struct RcVtbl {
    void (__fastcall *dtor)(void *self);
    void (__fastcall *release_dtor)(void *self);
};
struct RcObj {
    RcVtbl *vtbl;
    long    refs;
};
struct CTCInventoryItem {
    char    pad[0x0c];
    RcObj  *m_holder;
};
struct CEngineGraphic;
CEngineGraphic * __fastcall GetEngineGraphic(CTCInventoryItem *self)
{
    RcObj *p = self->m_holder;
    if (p)
        ++p->refs;
    CEngineGraphic *ret = (CEngineGraphic *)((char *)p + 0x25);
    if (p) {
        if (--p->refs == 0)
            p->vtbl->release_dtor(p);
    }
    return ret;
}