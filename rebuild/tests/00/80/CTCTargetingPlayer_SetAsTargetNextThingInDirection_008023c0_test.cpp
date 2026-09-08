#include <stdio.h>
enum Method{Default=0,Clockwise=2};unsigned long GetCurrentWorldFrame(){return 1234;}
struct Targeting{unsigned char base[0x3c];bool Next;unsigned char pad[3];Method NextMethod;
 unsigned long Frame;void SetAsTargetNextThingInDirection(Method);};
void Targeting::SetAsTargetNextThingInDirection(Method m){Next=true;NextMethod=m;Frame=GetCurrentWorldFrame();}
int main(){Targeting t;t.Next=false;t.NextMethod=Default;t.Frame=0;t.SetAsTargetNextThingInDirection(Clockwise);
 if(!t.Next||t.NextMethod!=Clockwise||t.Frame!=1234)return 1;printf("TARGET_NEXT_DIRECTION PASS\n");return 0;}