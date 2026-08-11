#include <cstdio>

struct CCreatureActionBase {
    virtual void v0() {}
    virtual int v1() { return 0; }
};

struct CSubObject {
    void RemoveByKey(int key);
};

struct CModeSitBase {
    char pad[0xd0];
    CSubObject sub;
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->sub.RemoveByKey(action->v1());
}

// --- test doubles ---
static int g_removed_key = -1;
void CSubObject::RemoveByKey(int key) { g_removed_key = key; }

struct MyAction : CCreatureActionBase {
    void v0() {}
    int v1() { return 4242; }
};

int main() {
    CModeSitBase* m = new CModeSitBase();
    MyAction a;
    m->RemoveModeWithAction(&a);
    // verify sub is at +0xd0
    unsigned long off = (unsigned long)((char*)&m->sub - (char*)m);
    if (off == 0xd0 && g_removed_key == 4242) {
        printf("REMOVEMODE_OK key=%d off=%lu\n", g_removed_key, off);
    } else {
        printf("FAIL off=%lu key=%d\n", off, g_removed_key);
    }
    delete m;
    return (off == 0xd0 && g_removed_key == 4242) ? 0 : 1;
}