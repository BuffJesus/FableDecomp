#include <stdio.h>
struct ListHead { ListHead* first; };
struct GlobalObj { char pad[0x68]; ListHead* list; };
struct CTCCreatureGroupBoastingCrowd { char pad[0x30]; void* arg; };
static int g_called = 0;
static void* g_last = 0;
void Callee(void* a){ g_called++; g_last = a; }
void OnHeroEnterBoastingArea_model(GlobalObj* g, CTCCreatureGroupBoastingCrowd* self)
{
    ListHead* head = g->list;
    if (head->first == head) return;
    Callee(self->arg);
}
int main(){
    GlobalObj g; ListHead h; g.list = &h;
    CTCCreatureGroupBoastingCrowd self; self.arg = (void*)0x1234;
    h.first = &h; g_called = 0;
    OnHeroEnterBoastingArea_model(&g, &self);
    if (g_called != 0) { printf("FAIL empty\n"); return 1; }
    ListHead other; h.first = &other; g_called = 0; g_last = 0;
    OnHeroEnterBoastingArea_model(&g, &self);
    if (g_called != 1 || g_last != (void*)0x1234) { printf("FAIL nonempty\n"); return 1; }
    printf("OK_00784690\n");
    return 0;
}