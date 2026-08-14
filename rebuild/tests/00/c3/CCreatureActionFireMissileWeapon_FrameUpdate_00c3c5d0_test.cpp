#include <cstdio>

struct Obj { char pad[0xc]; unsigned int counter; };

static int g_reported = 0;
static int g_lastCode = 0;

void report_stub(int code) { g_reported = 1; g_lastCode = code; }

bool FrameUpdate_model(Obj* self)
{
    unsigned int v = self->counter;
    if (v >= 0xFFFFFFFFu) {
        report_stub(-6);
        return false;
    }
    self->counter = v + 1;
    return true;
}

int main()
{
    Obj o; o.counter = 5;
    bool r = FrameUpdate_model(&o);
    if (!(r == true && o.counter == 6 && g_reported == 0)) { printf("FAIL1\n"); return 1; }

    o.counter = 0; r = FrameUpdate_model(&o);
    if (!(r == true && o.counter == 1)) { printf("FAIL2\n"); return 1; }

    o.counter = 0xFFFFFFFEu; r = FrameUpdate_model(&o);
    if (!(r == true && o.counter == 0xFFFFFFFFu)) { printf("FAIL3\n"); return 1; }

    o.counter = 0xFFFFFFFFu; g_reported = 0; r = FrameUpdate_model(&o);
    if (!(r == false && o.counter == 0xFFFFFFFFu && g_reported == 1 && g_lastCode == -6)) { printf("FAIL4\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}