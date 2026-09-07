#pragma optimize("gs",on)

struct IVisitor {
    virtual void f00();
    virtual void f01(void* a, void* b);   /* vtbl +0x4 */
    virtual void f02(void* a, void* b);   /* vtbl +0x8 */
};

struct CScrollingComponent {
    char pad[0x18];
    int  state;    /* +0x18 */
    char pad2[0x08];
    void* field24; /* +0x24 */
    void* field28; /* +0x28 */
};

struct ScopeGuard { char buf[0x18]; void init(int flag); };

extern void __stdcall SomeGlobalFunc(const void* arg);

void __fastcall CScrollingComponent_ChangeState(CScrollingComponent* self, void* edx, IVisitor* p)
{
    SomeGlobalFunc((const void*)0x122d70e);
    ScopeGuard g;
    g.init(0);
    int s = self->state;
    switch (s) {
    case 2:
        p->f01(self->field24, p);
        break;
    case 3:
        p->f02(self->field28, p);
        break;
    }
}