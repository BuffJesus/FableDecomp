#include <stdio.h>

struct CModeDragonRearUp;

struct CSomeState {
    char pad[0x10];
    unsigned char flag;   // +0x10
    void Notify(int a, CModeDragonRearUp* self);
};

void __fastcall DragonRear_BaseActivate(CModeDragonRearUp* self);

struct CModeDragonRearUp {
    void* vtbl;
    CSomeState* state;
    void OnActivate();
};

static int g_base_called = 0;
static int g_notify_called = 0;
static int g_notify_arg = -99;
static CSomeState* g_notify_self_state = 0;

void __fastcall DragonRear_BaseActivate(CModeDragonRearUp* self) {
    (void)self;
    g_base_called++;
}

void CSomeState::Notify(int a, CModeDragonRearUp* self) {
    (void)self;
    g_notify_called++;
    g_notify_arg = a;
    g_notify_self_state = this;
}

void CModeDragonRearUp::OnActivate() {
    DragonRear_BaseActivate(this);
    CSomeState* s = this->state;
    if (s->flag == 1) {
        s->Notify(0, this);
    }
}

int main() {
    CSomeState st;
    st.flag = 1;
    CModeDragonRearUp obj;
    obj.state = &st;

    obj.OnActivate();
    // flag==1: base called + notify called with arg 0, ecx==&st
    int ok1 = (g_base_called == 1) && (g_notify_called == 1)
              && (g_notify_arg == 0) && (g_notify_self_state == &st);

    g_base_called = g_notify_called = 0; g_notify_arg = -99; g_notify_self_state = 0;
    st.flag = 0;
    obj.OnActivate();
    // flag!=1: base called, notify NOT called
    int ok2 = (g_base_called == 1) && (g_notify_called == 0);

    if (ok1 && ok2) {
        printf("DRAGONREARUP_OK\n");
        return 0;
    }
    printf("FAIL b=%d n=%d arg=%d\n", g_base_called, g_notify_called, g_notify_arg);
    return 1;
}