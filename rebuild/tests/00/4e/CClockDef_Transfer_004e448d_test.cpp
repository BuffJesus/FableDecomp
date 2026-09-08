#include <stdio.h>
struct Context { int Calls; Context():Calls(0){} template<class T> void Transfer(T&){++Calls;} };

struct Map{};enum Hand{H};struct Def{Map Sound;Hand HandType;void Transfer(Context& c){c.Transfer(Sound);c.Transfer(HandType);}};
int main(){Context c;Def d;d.Transfer(c);if(c.Calls!=2)return 1;printf("CLOCK_TRANSFER PASS\n");return 0;}