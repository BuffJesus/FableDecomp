#include <cstdio>

struct CAnimComponentBase;

struct SubObj {
    int marker;
    void copyFrom(SubObj* src);
};

struct CAnimComponentBase {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual CAnimComponentBase* CreateInstance();
    char pad[4];
    SubObj sub;
};

struct CAnimComponentStrikeResponseAnim : public CAnimComponentBase {
    CAnimComponentBase* Clone() const;
};

// ---- stub externals used by Clone ----
static CAnimComponentBase* g_created = 0;

void CAnimComponentBase::v0() {}
void CAnimComponentBase::v1() {}
void CAnimComponentBase::v2() {}
void CAnimComponentBase::v3() {}
CAnimComponentBase* CAnimComponentBase::CreateInstance() {
    // hand back a preallocated fresh instance
    return g_created;
}
void SubObj::copyFrom(SubObj* src) {
    this->marker = src->marker + 100;
}

// The real function under test:
CAnimComponentBase* CAnimComponentStrikeResponseAnim::Clone() const {
    CAnimComponentBase* n = ((CAnimComponentBase*)this)->CreateInstance();
    n->sub.copyFrom(&((CAnimComponentBase*)this)->sub);
    return n;
}

int main() {
    CAnimComponentStrikeResponseAnim self;
    self.sub.marker = 7;

    CAnimComponentBase fresh;
    fresh.sub.marker = 0;
    g_created = &fresh;

    CAnimComponentBase* r = self.Clone();

    bool ok = (r == &fresh) && (r->sub.marker == 107);
    if (ok) {
        printf("CLONE_STRIKE_OK\n");
        return 0;
    }
    printf("CLONE_STRIKE_FAIL r=%p marker=%d\n", (void*)r, r ? r->sub.marker : -1);
    return 1;
}