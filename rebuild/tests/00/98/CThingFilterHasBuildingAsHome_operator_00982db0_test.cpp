#include <stdio.h>
struct Thing{};struct Creature{Thing* Home;Thing* GetHomeBuilding()const{return Home;}};
struct Filter{Thing* HomeBuilding;bool operator()(const Creature*)const;};
bool Filter::operator()(const Creature* creature)const{return creature->GetHomeBuilding()==HomeBuilding;}
int main(){Thing home,other;Creature creature;creature.Home=&home;Filter filter;filter.HomeBuilding=&home;
 if(!filter(&creature))return 1;filter.HomeBuilding=&other;if(filter(&creature))return 2;
 printf("HOME_BUILDING_FILTER PASS\n");return 0;}