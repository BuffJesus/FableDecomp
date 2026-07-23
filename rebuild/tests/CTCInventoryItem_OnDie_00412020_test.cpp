#include <cstdio>

static int g_vtbl = 0;
static int freed = 0;
static int cleanup = 0;
static int subdtor = 0;
static int base = 0;

void SomeCleanup(void* self){ cleanup++; }
void SubMemberDtor(void* sub){ subdtor++; }
void FreeMem(void* p){ freed++; }
void BaseOnDie(void* self){ base++; }

struct CTCInventoryItem {
    void* vtbl;
    char pad0[0x0C];
    void* ptr10;
    char pad1[0x0C];
    char sub20[0x04];
};

void CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    self->vtbl = &g_vtbl;
    SomeCleanup(self);
    SubMemberDtor(&self->sub20);
    if (self->ptr10) {
        FreeMem(self->ptr10);
    }
    BaseOnDie(self);
}

int main(){
    CTCInventoryItem a;
    int dummy = 5;
    a.ptr10 = &dummy;
    CTCInventoryItem_OnDie(&a);
    if (cleanup==1 && subdtor==1 && freed==1 && base==1 && a.vtbl==(void*)&g_vtbl) {
        CTCInventoryItem b;
        b.ptr10 = 0;
        CTCInventoryItem_OnDie(&b);
        if (freed==1 && base==2) {
            std::printf("CTCInventoryItem_00412020_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}