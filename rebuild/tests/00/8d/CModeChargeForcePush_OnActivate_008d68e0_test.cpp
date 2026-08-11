#include <cstdio>

static int g_base_calls = 0;
static int g_notify_calls = 0;
static int g_notify_a = -1;
static int g_notify_b = -1;
static void* g_notify_this = 0;

struct CSomeState {
    char pad0[0x10];
    unsigned char flag;
    void Notify(int a, int b);
};

void CSomeState::Notify(int a, int b) {
    ++g_notify_calls;
    g_notify_a = a;
    g_notify_b = b;
    g_notify_this = this;
}

void __fastcall BaseMode_OnActivate(void* self) {
    (void)self;
    ++g_base_calls;
}

struct CModeChargeForcePush {
    void* vtbl;
    CSomeState* state;
    void OnActivate();
};

void CModeChargeForcePush::OnActivate()
{
    BaseMode_OnActivate(this);
    CSomeState* s = this->state;
    if (s->flag == 1)
        s->Notify(1, 0);
}

int main() {
    CSomeState st;
    st.flag = 1;
    CModeChargeForcePush m;
    m.vtbl = 0;
    m.state = &st;

    m.OnActivate();
    bool ok = (g_base_calls == 1) && (g_notify_calls == 1) &&
              (g_notify_a == 1) && (g_notify_b == 0) &&
              (g_notify_this == (void*)&st);

    st.flag = 0;
    m.OnActivate();
    ok = ok && (g_base_calls == 2) && (g_notify_calls == 1);

    if (ok) printf("FORCEPUSH_ONACTIVATE_OK\n");
    else printf("FAIL b=%d n=%d a=%d bb=%d\n", g_base_calls, g_notify_calls, g_notify_a, g_notify_b);
    return ok ? 0 : 1;
}