#include <stdio.h>
struct Context { int Calls; Context():Calls(0){} template<class T> void Transfer(T&){++Calls;} };

struct Vec{};struct Str{};struct Def{Vec AvailableCarrySlots;Str OverriddenDummyObject;void Transfer(Context& c){c.Transfer(AvailableCarrySlots);c.Transfer(OverriddenDummyObject);}};
int main(){Context c;Def d;d.Transfer(c);if(c.Calls!=2)return 1;printf("CARRYING_TRANSFER PASS\n");return 0;}