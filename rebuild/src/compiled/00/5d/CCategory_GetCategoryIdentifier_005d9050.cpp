typedef int EInventoryCategoryIdentifier;

struct RefObj {
    void** vtbl;
    int    refcnt;
    char   pad[0x40 - 8];
    int    catId;
};

struct CCategory {
    char pad[0xc];
    RefObj* m0c;
};

struct SmartRef {
    RefObj* p;
    SmartRef(RefObj* q) : p(q) { if (p) p->refcnt++; }
    ~SmartRef() {
        if (--p->refcnt == 0) {
            typedef void (__fastcall *Del)(RefObj*);
            ((Del)(p->vtbl[1]))(p);
        }
    }
};

EInventoryCategoryIdentifier __fastcall GetCategoryIdentifier(CCategory* self)
{
    SmartRef ref(self->m0c);
    EInventoryCategoryIdentifier id = self->m0c->catId;
    return id;
}