#include <stdio.h>
struct Filter{virtual bool AcceptsGroup(long)const{return true;}Filter* Clone()const;};
Filter* Filter::Clone()const{return new Filter;}
int main(){Filter original;Filter* copy=original.Clone();if(copy==0 || copy==&original || !copy->AcceptsGroup(7))return 1;
 delete copy;printf("ANIM_FILTER_ANY_GROUP_CLONE PASS\n");return 0;}