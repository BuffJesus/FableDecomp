#include <cstdio>

struct Action {
    virtual int GetSomething() { return 42; }
};

struct Sub { int last; };

static void SubCall(Sub* s, int arg) { s->last = arg; }

struct Mode {
    unsigned int flags;
    char pad[0x1c];
    Sub sub;
};

static void RemoveModeWithAction(Mode* self, Action* action)
{
    self->flags |= 8;
    int r = action->GetSomething();
    SubCall(&self->sub, r);
}

int main()
{
    Mode m; m.flags = 1; m.sub.last = 0;
    Action a;
    RemoveModeWithAction(&m, &a);
    if ((m.flags & 8) && (m.flags & 1) && m.sub.last == 42)
        printf("PARITY_OK\n");
    else
        printf("FAIL flags=%u last=%d\n", m.flags, m.sub.last);
    return 0;
}