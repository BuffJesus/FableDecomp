#include <stdio.h>
static int finished=0,notification=0;
struct Interface { void FinishCurrentCutFade(){finished=1;}
 void NotifyCutFadeEnded(int fadeId){notification=fadeId;} void EndCutFade(); };
void Interface::EndCutFade(){FinishCurrentCutFade();NotifyCutFadeEnded(0x104);}
int main(){Interface value;value.EndCutFade();
 if(finished!=1 || notification!=0x104) return 1;
 printf("END_CUT_FADE_0104 PASS\n");return 0;}