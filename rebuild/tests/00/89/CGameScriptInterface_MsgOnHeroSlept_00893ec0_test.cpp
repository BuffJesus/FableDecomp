#include <cstdio>
static bool Query(bool found,bool day,bool& out){if(found){out=day;return true;}return false;}
int main(){bool out=false;if(!Query(true,true,out)||!out)return 1;out=true;if(Query(false,false,out)||!out)return 2;puts("MSG_HERO_SLEPT_PASS");return 0;}