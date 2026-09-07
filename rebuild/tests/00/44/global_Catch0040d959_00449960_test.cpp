#include <stdio.h>
struct State{int index;};static int Forward(State* s){return s->index;}
int main(){State s={7};if(Forward(&s)!=7)return 1;s.index=-3;if(Forward(&s)!=-3)return 2;puts("TINY_CATCHES_PASS");return 0;}