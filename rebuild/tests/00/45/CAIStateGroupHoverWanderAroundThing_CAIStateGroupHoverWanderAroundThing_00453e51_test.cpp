#include <cstdio>
static int g_member_calls = 0;
static int g_base_calls = 0;
struct Member { int flag; };
struct Base { void* vt; };
void Member_dtor(Member* m){ m->flag = 1; g_member_calls++; }
void Base_dtor(Base* b){ g_base_calls++; }
static void* g_vtbl_data[1] = {0};
struct Obj { void* vt; char pad[0x24]; Member* m28ignore; };
void Obj_dtor(Obj* self){
    Member_dtor((Member*)((char*)self + 0x28));
    self->vt = (void*)g_vtbl_data;
    Base_dtor((Base*)self);
}
int main(){
    unsigned char buf[0x40];
    for (int i=0;i<0x40;i++) buf[i]=0;
    Obj* o = (Obj*)buf;
    Obj_dtor(o);
    Member* m = (Member*)(buf + 0x28);
    if (g_member_calls==1 && g_base_calls==1 && m->flag==1 && o->vt==(void*)g_vtbl_data){
        std::printf("CAIStateGroup_HoverWanderAroundThing_00453e51_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL m=%d b=%d\n", g_member_calls, g_base_calls);
    return 1;
}