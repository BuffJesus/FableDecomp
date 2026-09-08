#include <stdio.h>
struct Def{unsigned char base[0x94];long Timer;};
struct Object{unsigned char base[0xd0];Def* PDef;long Timer;bool DiesOff;void ResetDieOffTimer();};
void Object::ResetDieOffTimer(){Timer=PDef->Timer;DiesOff=Timer>0;}
int main(){Def d;Object o;o.PDef=&d;d.Timer=30;o.ResetDieOffTimer();if(o.Timer!=30||!o.DiesOff)return 1;
 d.Timer=0;o.ResetDieOffTimer();if(o.Timer||o.DiesOff)return 2;d.Timer=-5;o.ResetDieOffTimer();
 if(o.Timer!=-5||o.DiesOff)return 3;printf("THING_OBJECT_DIE_OFF_RESET PASS\n");return 0;}