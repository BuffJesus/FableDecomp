#include "engine/CAnimComponentCombatComboChain.h"
#include <cstdio>
static void f(CAnimComponentCombatComboChain* self){ self->ComboID=-999; self->ComboStage=-777; }
int main(){ CAnimComponentCombatComboChain o; f(&o); if(o.ComboStage!=-777||o.ComboID!=-999){ std::printf("005db2b0_TEST FAIL\n"); return 1;}
 std::printf("005db2b0_TEST PASS\n"); return 0;}