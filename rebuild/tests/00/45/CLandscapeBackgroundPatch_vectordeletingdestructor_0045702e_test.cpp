#include <cstdio>
static int g_dtor = 0;
static int g_freed = 0;
extern "C" void operator_delete_stub(void* p){ g_freed++; (void)p; }
struct CLandscapeBackgroundPatch {
    int x;
    void dtor(){ g_dtor++; }
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator_delete_stub(this);
    return this;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.x = 7;
    g_dtor=0; g_freed=0;
    void* r1 = obj.vector_deleting_destructor(0);
    if (r1 != &obj || g_dtor != 1 || g_freed != 0){ std::printf("FAIL noflag\n"); return 1; }
    g_dtor=0; g_freed=0;
    void* r2 = obj.vector_deleting_destructor(1);
    if (r2 != &obj || g_dtor != 1 || g_freed != 1){ std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045702e_TEST PASS\n");
    return 0;
}