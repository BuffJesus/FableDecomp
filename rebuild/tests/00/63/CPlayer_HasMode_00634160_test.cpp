#include <list>
#include <algorithm>
#include <stdio.h>
int main(){std::list<int> v;v.push_back(2);v.push_back(7);if(std::find(v.begin(),v.end(),7)==v.end()||std::find(v.begin(),v.end(),3)!=v.end())return 1;printf("PLAYER_HAS_MODE PASS\n");return 0;}