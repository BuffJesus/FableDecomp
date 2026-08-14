#include <cstdio>

struct Creature { int inCombat; int removed; };
struct Mgr { int lastRemoved; };

char __fastcall Creature_IsCombatant(Creature* c) { return (char)c->inCombat; }
void __fastcall Mgr_Remove(Mgr* self, void* edx, Creature* c) { self->lastRemoved = 1; c->removed = 1; }

void __fastcall Deregister(Mgr* self, void* edx, Creature* c)
{
    if (Creature_IsCombatant(c))
        Mgr_Remove(self, 0, c);
}

int main()
{
    Mgr m; Creature a, b;

    m.lastRemoved = 0; a.inCombat = 1; a.removed = 0;
    Deregister(&m, 0, &a);
    if (m.lastRemoved != 1 || a.removed != 1) { printf("FAIL1\n"); return 1; }

    m.lastRemoved = 0; b.inCombat = 0; b.removed = 0;
    Deregister(&m, 0, &b);
    if (m.lastRemoved != 0 || b.removed != 0) { printf("FAIL2\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}