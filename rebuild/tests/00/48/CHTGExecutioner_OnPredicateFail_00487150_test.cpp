#include <stdio.h>
struct Sub { char pad[0x79c]; unsigned char flag; };
struct CHTG_Executioner { char pad0[0xc]; Sub* sub; };
static int g_helper_arg = -1;
void HelperModel(int x){ g_helper_arg = x; }
void OnPredicateFail_b_model(CHTG_Executioner* self){ HelperModel(0x12); self->sub->flag = 0; }
int main(){
    Sub s; s.flag = 0xAB;
    CHTG_Executioner e; e.sub = &s;
    OnPredicateFail_b_model(&e);
    if (g_helper_arg != 0x12) { printf("FAIL arg\n"); return 1; }
    if (s.flag != 0) { printf("FAIL flag\n"); return 1; }
    s.flag = 0x7F;
    OnPredicateFail_b_model(&e);
    if (s.flag != 0) { printf("FAIL flag2\n"); return 1; }
    printf("OK_00487150\n");
    return 0;
}