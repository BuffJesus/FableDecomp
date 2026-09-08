#include <stdio.h>
struct E{const char*name;float time;};int main(){E e={"hit",0.5f};if(e.name[0]!='h'||e.time!=0.5f)return 1;printf("ANIM_TIMING_EVENT_WRITE PASS\n");return 0;}