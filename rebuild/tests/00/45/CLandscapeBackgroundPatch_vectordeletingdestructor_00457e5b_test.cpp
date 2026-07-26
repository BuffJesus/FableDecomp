#include <cstdio>
static int g_dtor_called = 0;
static int g_delete_called = 0;
extern "C" void __cdecl eng_operator_delete(void* p){ (void)p; g_delete_called++; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    void Dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void CLandscapeBackgroundPatch::Dtor(){ g_dtor_called++; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1) {
        eng_operator_delete(this);
    }
    return this;
}

int main(){
    CLandscapeBackgroundPatch obj;
    obj.vftable = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    if (r1 != &obj) { std::printf("FAIL ret1\n"); return 1; }
    if (g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    void* r2 = obj.vector_deleting_destructor(1);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 2 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457e5b_TEST PASS\n");
    return 0;
}