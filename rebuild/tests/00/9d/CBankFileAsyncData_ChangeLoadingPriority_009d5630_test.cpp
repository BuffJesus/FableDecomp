#include <cstdio>
typedef int EThreadedFilePriority;

struct Inner { int id; };

static int g_called = 0;
static Inner* g_item = 0;
static EThreadedFilePriority g_prio = -1;

struct Consumer {
    char pad[0x100];
    void SetPriority(Inner* item, EThreadedFilePriority prio) {
        g_called++;
        g_item = item;
        g_prio = prio;
    }
};

struct Manager {
    char pad[0x110];
    Consumer* field_110;
};

struct CBankFileAsyncData {
    char pad0[0x18];
    Inner* field_18;
    char pad1[0x28 - 0x1c];
    Manager* field_28;
};

void ChangeLoadingPriority(CBankFileAsyncData* self, EThreadedFilePriority prio)
{
    Inner* item = self->field_18;
    if (item) {
        Consumer* c = self->field_28->field_110;
        c->SetPriority(item, prio);
    }
}

int main() {
    Consumer cons;
    Manager mgr; mgr.field_110 = &cons;
    Inner item; item.id = 7;

    CBankFileAsyncData a;
    a.field_18 = &item;
    a.field_28 = &mgr;

    // Case 1: field_18 non-null -> calls SetPriority(item, prio)
    ChangeLoadingPriority(&a, 42);
    if (g_called != 1 || g_item != &item || g_prio != 42) { printf("BAD1\n"); return 1; }

    // Case 2: field_18 null -> no call
    g_called = 0;
    a.field_18 = 0;
    ChangeLoadingPriority(&a, 99);
    if (g_called != 0) { printf("BAD2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}