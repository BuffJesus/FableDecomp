// Independent behaviour model for CMiniMapDisplay::Initialise
// Verifies: (1) the subobject at self+0x20 gets its Reset called,
//           (2) the global flag is cleared to 0,
//           (3) the float arg is ignored.
#include <cstdio>
#include <cstring>

struct Sub { int resetCalled; };

struct CMiniMapDisplay {
    char pad[0x20];
    // Sub lives AT offset 0x20 (inline)
};

static unsigned char g_flag = 0xFF;

static void Sub_Reset(Sub* s) { s->resetCalled = 1; }

static void Initialise_model(CMiniMapDisplay* self, float /*unused*/)
{
    Sub* p = reinterpret_cast<Sub*>(reinterpret_cast<char*>(self) + 0x20);
    Sub_Reset(p);
    g_flag = 0;
}

int main()
{
    // allocate block big enough for pad[0x20] + a Sub
    char buf[0x20 + sizeof(Sub)];
    memset(buf, 0, sizeof(buf));
    CMiniMapDisplay* self = reinterpret_cast<CMiniMapDisplay*>(buf);
    Sub* sub = reinterpret_cast<Sub*>(buf + 0x20);
    sub->resetCalled = 0;
    g_flag = 0xFF;

    Initialise_model(self, 3.14f);

    bool ok = (sub->resetCalled == 1) && (g_flag == 0);
    if (ok) printf("INIT_OK\n");
    else    printf("INIT_FAIL\n");
    return 0;
}