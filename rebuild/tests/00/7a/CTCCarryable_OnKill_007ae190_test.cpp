#include <cstdio>

struct CTCCarryableNode {
    void*        f0;   /* +0x0 */
    unsigned int f4;   /* +0x4 */
    void*        f8;   /* +0x8 */
    void*        fc;   /* +0xc */
};

struct CTCCarryable {
    CTCCarryableNode* m0;   /* +0x0 */
    unsigned int      m4;   /* +0x4 */
    void Detach(unsigned int a);
};

static int   g_detach_called = 0;
static unsigned int g_detach_arg = 0;

void CTCCarryable::Detach(unsigned int a) {
    g_detach_called++;
    g_detach_arg = a;
}

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->m4 != 0) {
        self->Detach(self->m0->f4);
        self->m0->f8 = self->m0;
        self->m0->f4 = 0;
        self->m0->fc = self->m0;
        self->m4 = 0;
    }
}

int main() {
    CTCCarryableNode node;
    CTCCarryable o;

    /* active path: m4 != 0 */
    node.f0 = (void*)0xAAAA;
    node.f4 = 77;
    node.f8 = (void*)0xBBBB;
    node.fc = (void*)0xCCCC;
    o.m0 = &node;
    o.m4 = 5;

    CTCCarryable_OnKill(&o);

    if (g_detach_called != 1) { std::printf("FAIL detach_called\n"); return 1; }
    if (g_detach_arg != 77)   { std::printf("FAIL detach_arg\n"); return 1; }
    if (node.f8 != (void*)&node) { std::printf("FAIL f8\n"); return 1; }
    if (node.f4 != 0)            { std::printf("FAIL f4\n"); return 1; }
    if (node.fc != (void*)&node) { std::printf("FAIL fc\n"); return 1; }
    if (o.m4 != 0)              { std::printf("FAIL m4\n"); return 1; }

    /* null path: m4 == 0 -> no-op */
    g_detach_called = 0;
    node.f4 = 123;
    node.f8 = (void*)0x1;
    node.fc = (void*)0x2;
    o.m0 = &node;
    o.m4 = 0;

    CTCCarryable_OnKill(&o);

    if (g_detach_called != 0)   { std::printf("FAIL null detach\n"); return 1; }
    if (node.f4 != 123)         { std::printf("FAIL null f4\n"); return 1; }
    if (node.f8 != (void*)0x1)  { std::printf("FAIL null f8\n"); return 1; }
    if (node.fc != (void*)0x2)  { std::printf("FAIL null fc\n"); return 1; }
    if (o.m4 != 0)              { std::printf("FAIL null m4\n"); return 1; }

    std::printf("CTCCARRYABLE_ONKILL_0047C880_TEST PASS\n");
    return 0;
}