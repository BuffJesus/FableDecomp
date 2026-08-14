#include <stdio.h>

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

static int g_deleted = 0;

void __fastcall FakeDel(RefObj* p) {
    g_deleted = 1;
}

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

int main() {
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&FakeDel;

    RefObj obj;
    obj.vtbl = vt;
    obj.refcnt = 5;      // held elsewhere: not last ref
    obj.catId = 42;

    CCategory cat;
    cat.m0c = &obj;

    int r = GetCategoryIdentifier(&cat);
    // refcount 5 -> +1 =6 -> -1 =5, not zero: no delete
    if (r == 42 && obj.refcnt == 5 && g_deleted == 0) {
        // last-ref delete path
        obj.refcnt = 0;   // copy makes it 1, dtor makes it 0 -> delete
        g_deleted = 0;
        int r2 = GetCategoryIdentifier(&cat);
        if (r2 == 42 && g_deleted == 1) {
            printf("PARITY_OK\n");
            return 0;
        }
    }
    printf("FAIL\n");
    return 1;
}