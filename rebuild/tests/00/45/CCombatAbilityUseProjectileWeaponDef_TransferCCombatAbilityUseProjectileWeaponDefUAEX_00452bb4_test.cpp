#include <cstdio>

struct CCombatAbilityUseProjectileWeaponDef {
    unsigned char _tail[0x48];
};

struct CPersistContext {
    int calls;
    void* args[3];
    void PersistA(void* p){ args[calls++]=p; }
    void PersistB(void* p){ args[calls++]=p; }
    void PersistC(void* p){ args[calls++]=p; }
};

static void Transfer(CCombatAbilityUseProjectileWeaponDef* self, CPersistContext* ctx)
{
    ctx->PersistA(self->_tail + 0x40);
    ctx->PersistB(self->_tail + 0x44);
    ctx->PersistC(self->_tail + 0x28);
}

int main(){
    CCombatAbilityUseProjectileWeaponDef obj;
    CPersistContext ctx; ctx.calls=0;
    Transfer(&obj, &ctx);
    if (ctx.calls==3 &&
        ctx.args[0]==(void*)(obj._tail+0x40) &&
        ctx.args[1]==(void*)(obj._tail+0x44) &&
        ctx.args[2]==(void*)(obj._tail+0x28)) {
        std::printf("CCombatAbilityUseProjectileWeaponDef_00452bb4_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}