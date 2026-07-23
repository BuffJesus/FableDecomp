#include <cstdio>

static int g_steps = 0;

struct CAIStateGroup_CreatureUseCoverBase {
    unsigned int vtbl;
    char pad[0x14];
    char member18[4];
};

void __fastcall sub_5992f0(void* self){ g_steps += 1; (void)self; }
void __fastcall sub_599ee0(void* member){ g_steps += 10; (void)member; }
void __fastcall base_dtor(void* self){ g_steps += 100; (void)self; }

void __fastcall CAIStateGroup_CreatureUseCoverBase_dtor(CAIStateGroup_CreatureUseCoverBase* self)
{
    self->vtbl = 0x122d06c;
    sub_5992f0(self);
    sub_599ee0(&self->member18);
    base_dtor(self);
}

int main(){
    CAIStateGroup_CreatureUseCoverBase obj;
    obj.vtbl = 0;
    CAIStateGroup_CreatureUseCoverBase_dtor(&obj);
    if (g_steps != 111) { std::printf("FAIL steps=%d\n", g_steps); return 1; }
    if (obj.vtbl != 0x122d06c) { std::printf("FAIL vtbl\n"); return 1; }
    std::printf("CAIStateGroup_CreatureUseCoverBase_00401630_TEST PASS\n");
    return 0;
}