#include <stdio.h>

struct Creature {
    bool combatant;
    bool IsCombatant() { return combatant; }
};

struct CombatMgr {
    int removed;
    void RemoveCombatant(Creature* c) { removed++; }
    void Deregister(Creature* c) {
        if (c->IsCombatant())
            this->RemoveCombatant(c);
    }
};

int main() {
    CombatMgr m; m.removed = 0;
    Creature a; a.combatant = true;
    Creature b; b.combatant = false;
    m.Deregister(&b);
    if (m.removed != 0) { printf("FAIL: removed non-combatant\n"); return 1; }
    m.Deregister(&a);
    if (m.removed != 1) { printf("FAIL: did not remove combatant\n"); return 1; }
    printf("BEHAVIOR_OK\n");
    return 0;
}