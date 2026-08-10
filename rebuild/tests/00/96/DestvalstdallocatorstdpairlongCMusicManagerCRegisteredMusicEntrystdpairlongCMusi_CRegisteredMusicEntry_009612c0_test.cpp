#include <stdio.h>

struct Sub { char pad[4]; };
struct Owner { char pad[0xa8]; Sub sub; };

static int g_sub_init = 0;
static int g_tail = 0;
static Sub* g_sub_arg = 0;
static Owner* g_tail_arg = 0;

void Sub_Init_model(Sub* s){ g_sub_init++; g_sub_arg = s; }
void Tail_model(Owner* o){ g_tail++; g_tail_arg = o; }

void DestVal_A8_J_model(Owner* self){ Sub_Init_model(&self->sub); Tail_model(self); }

int main(){
    Owner o;
    DestVal_A8_J_model(&o);
    if(g_sub_init!=1){ printf("FAIL init count\n"); return 1; }
    if(g_tail!=1){ printf("FAIL tail count\n"); return 1; }
    if((char*)g_sub_arg != (char*)&o + 0xa8){ printf("FAIL sub offset\n"); return 1; }
    if(g_tail_arg != &o){ printf("FAIL tail arg\n"); return 1; }
    printf("OK_009612c0\n");
    return 0;
}