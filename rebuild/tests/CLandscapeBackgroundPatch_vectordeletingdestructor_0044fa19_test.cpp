#include <cstdio>
static void* g_last_deleted = 0;
struct CLandscapeBackgroundPatch { void* vtbl; };
void* g_vtbl;
static bool g_dtor_called = false;
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch*){ g_dtor_called = true; }
void __cdecl eng_delete(void* p){ g_last_deleted = p; }
static char g_buf[0x60];
void* __cdecl eng_new(unsigned int){ return g_buf; }
void* __fastcall CLandscapeBackgroundPatch_ctor(CLandscapeBackgroundPatch* s){ s->vtbl=0; return s; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int, unsigned char flags)
{
    self->vtbl = &g_vtbl;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_delete(self);
    return self;
}
void* __fastcall CLandscapeBackgroundPatch_new_closure()
{
    void* p = eng_new(0x60);
    if (p) return CLandscapeBackgroundPatch_ctor((CLandscapeBackgroundPatch*)p);
    return 0;
}

int main(){
    CLandscapeBackgroundPatch obj; obj.vtbl=0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj,0,0);
    if(r!=&obj || g_last_deleted!=0 || !g_dtor_called){ std::printf("FAIL noflag\n"); return 1; }
    g_last_deleted=0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj,0,1);
    if(r!=&obj || g_last_deleted!=&obj){ std::printf("FAIL flag\n"); return 1; }
    void* c = CLandscapeBackgroundPatch_new_closure();
    if(c!=g_buf){ std::printf("FAIL closure\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044fa19_TEST PASS\n");
    return 0;
}