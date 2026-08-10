#include <cstdio>
struct Inner { char pad[0x58]; unsigned char flag; };
struct CTCQuestCard { char pad0c[0x0c]; Inner* f0c; char pad10[0x04]; int f14; };
static bool model(CTCQuestCard* self) {
    if (self->f14 > -1) return true;
    if (self->f0c->flag == 0) return true;
    return false;
}
int main(){
    Inner inner; inner.flag = 0;
    Inner* pin = &inner;
    CTCQuestCard card; card.f0c = pin;
    CTCQuestCard* pc = &card;
    pc->f14 = 5;
    if (model(pc) != true) { printf("FAIL1\n"); return 1; }
    pc->f14 = -1; pin->flag = 1;
    if (model(pc) != false) { printf("FAIL2\n"); return 1; }
    pc->f14 = -1; pin->flag = 0;
    if (model(pc) != true) { printf("FAIL3\n"); return 1; }
    pc->f14 = -5; pin->flag = 1;
    if (model(pc) != false) { printf("FAIL4\n"); return 1; }
    printf("OK_007022c0\n");
    return 0;
}