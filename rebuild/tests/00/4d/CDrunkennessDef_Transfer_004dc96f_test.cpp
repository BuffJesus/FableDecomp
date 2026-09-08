#include <stdio.h>
struct Context{int Calls;Context():Calls(0){}void Transfer(const char*,float& value,const float& fallback){++Calls;if(value<0)value=fallback;}};
struct Def{float DrunkennessThresholdMult;void Transfer(Context& c){const float fallback=0;c.Transfer("DrunkennessThresholdMult",DrunkennessThresholdMult,fallback);}};
int main(){Context c;Def d={-1};d.Transfer(c);if(c.Calls!=1||d.DrunkennessThresholdMult!=0)return 1;printf("DRUNKENNESS_TRANSFER PASS\n");return 0;}