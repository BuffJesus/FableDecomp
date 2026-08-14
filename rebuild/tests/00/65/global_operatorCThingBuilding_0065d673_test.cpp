#include <cstdio>

// Behaviour model: operator== compares the resolved raw pointers of two smart pointers.
struct Thing { int v; };

struct CIP { Thing* target; Thing* Get() { return target; } };

static bool eq_model(CIP* a, CIP* b) {
    return a->Get() == b->Get();
}

int main() {
    Thing t1, t2;
    CIP a, b, c;
    a.target = &t1;
    b.target = &t1;   // same target -> equal
    c.target = &t2;   // different target -> not equal

    bool r_eq  = eq_model(&a, &b);   // expect true
    bool r_neq = eq_model(&a, &c);   // expect false

    // null-vs-null equality
    CIP n1, n2; n1.target = 0; n2.target = 0;
    bool r_null = eq_model(&n1, &n2); // expect true

    if (r_eq == true && r_neq == false && r_null == true) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL %d %d %d\n", (int)r_eq, (int)r_neq, (int)r_null);
    }
    return 0;
}