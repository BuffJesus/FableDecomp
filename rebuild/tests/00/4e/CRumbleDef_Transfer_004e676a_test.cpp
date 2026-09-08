#include <stdio.h>
struct Context { int Calls; Context():Calls(0){} template<class T> void Transfer(T&){++Calls;} };

struct MapA{};struct MapB{};struct Def{MapA QuakeIntensities;MapB QuakeDurations;void Transfer(Context& c){c.Transfer(QuakeIntensities);c.Transfer(QuakeDurations);}};
int main(){Context c;Def d;d.Transfer(c);if(c.Calls!=2)return 1;printf("RUMBLE_TRANSFER PASS\n");return 0;}