#include <cstdio>
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void CLandscapeBackgroundPatch_dtor();
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};
static int g_dtor = 0;
static int g_del = 0;
void CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_dtor(){ g_dtor++; }
void __cdecl op_delete(void* p){ (void)p; g_del++; }
void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->CLandscapeBackgroundPatch_dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}
int main(){
    CLandscapeBackgroundPatch obj;
    void* r = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL nodelete\n"); return 1; }
    r = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r != &obj || g_dtor != 2 || g_del != 1){ std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456515_TEST PASS\n");
    return 0;
}