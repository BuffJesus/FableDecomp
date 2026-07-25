#include <cstdio>

struct CLandscapeBackgroundPatch { void** vftable; };
void* g_CLandscapeBackgroundPatch_vftable;
int g_deleted = 0;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch*) {}
void engine_operator_delete(void* p) { (void)p; g_deleted = 1; }

extern void* __fastcall CLandscapeBackgroundPatch_vector_dtor(
    CLandscapeBackgroundPatch*, int, unsigned int);

int main() {
    CLandscapeBackgroundPatch obj; obj.vftable = 0;
    void* r = CLandscapeBackgroundPatch_vector_dtor(&obj, 0, 1u);
    if (r == &obj && obj.vftable == (void**)&g_CLandscapeBackgroundPatch_vftable && g_deleted)
        printf("OK_0046642a\n");
    return 0;
}