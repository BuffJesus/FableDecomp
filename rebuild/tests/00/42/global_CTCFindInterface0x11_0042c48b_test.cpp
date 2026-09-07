#include <stdio.h>
struct Pair { int key; int value; };
static bool Find(bool present,Pair* begin,Pair* end,int* output){if(!present)return false;Pair* p=begin;while(p!=end&&p->key<0x11)++p;if(p==end||p->key>0x11)p=end;*output=p->value;return true;}
int main(){Pair a[4]={{1,10},{0x11,99},{20,30},{0,77}};int out=-1;if(Find(false,a,a+3,&out)||out!=-1)return 1;if(!Find(true,a,a+3,&out)||out!=99)return 2;if(!Find(true,a+2,a+3,&out)||out!=77)return 3;puts("FIND_INTERFACE_PASS");return 0;}