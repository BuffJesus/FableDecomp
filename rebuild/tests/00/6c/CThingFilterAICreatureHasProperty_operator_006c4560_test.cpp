#include <stdio.h>
struct Creature{virtual void Reserved(){} virtual unsigned long GetProperty()const{return 7;}};
struct Filter{unsigned long Property;bool operator()(const Creature*)const;};
bool Filter::operator()(const Creature* creature)const{return creature->GetProperty()==Property;}
int main(){Creature creature;Filter filter;filter.Property=7;if(!filter(&creature))return 1;
 filter.Property=8;if(filter(&creature))return 2;
 printf("AI_CREATURE_PROPERTY_FILTER PASS\n");return 0;}