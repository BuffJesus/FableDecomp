#include <cstdio>

struct Sub {
    void* first;
    char pad[0x20];
    void* f24;
};
struct CCharString;
struct CTCInventoryExperience {
    void* head;
    Sub   sub;
};

// Model of the engine callee: return a chosen pointer.
static void* g_found = 0;
void* __fastcall Sub_Find(Sub* self, void* edx, CCharString* p) {
    (void)self; (void)edx; (void)p;
    return g_found;
}

void* __fastcall IsTutorialPlayed(CTCInventoryExperience* self, void* edx, CCharString* p, CCharString* q)
{
    Sub* s = &self->sub;
    void* r = Sub_Find(s, 0, p);
    if (r != s->first) {
        ((Sub*)r)->f24 = q;
    }
    return r;
}

int main() {
    CTCInventoryExperience obj;
    obj.head = 0;
    obj.sub.first = (void*)0x1000;   // end/sentinel
    obj.sub.f24 = 0;
    Sub node;
    node.first = 0;
    node.f24 = 0;

    CCharString* q = (CCharString*)0xABCD;

    // Case 1: found != first -> writes q into found->f24, returns found.
    g_found = &node;
    void* r1 = IsTutorialPlayed(&obj, 0, 0, q);
    bool ok1 = (r1 == &node) && (node.f24 == q);

    // Case 2: found == first (sentinel) -> no write, returns first.
    g_found = obj.sub.first;
    node.f24 = 0;
    void* r2 = IsTutorialPlayed(&obj, 0, 0, q);
    bool ok2 = (r2 == obj.sub.first) && (node.f24 == 0);

    if (ok1 && ok2) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    }
    return 0;
}