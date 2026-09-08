#include <stdio.h>
struct Context { int Calls; Context():Calls(0){} template<class T> void Transfer(T&){++Calls;} };

struct Def { bool AddTankards, HighQualityTankards; void Transfer(Context& c){c.Transfer(AddTankards);c.Transfer(HighQualityTankards);} };
int main(){Context c;Def d={true,false};d.Transfer(c);if(c.Calls!=2)return 1;printf("TAVERN_TRANSFER PASS\n");return 0;}