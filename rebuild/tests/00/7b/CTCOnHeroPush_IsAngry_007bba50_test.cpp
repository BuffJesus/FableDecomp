#include <stdio.h>
float g_OnHeroPushAngryDistance = 3.0f;
struct CTCOnHeroPush { unsigned char base[0x0c]; float DistPushed; bool IsAngry() const; };
bool CTCOnHeroPush::IsAngry() const { return DistPushed > g_OnHeroPushAngryDistance; }
int main() { CTCOnHeroPush push;
 push.DistPushed=3.5f;if(!push.IsAngry())return 1;
 push.DistPushed=3.0f;if(push.IsAngry())return 2;
 printf("ON_HERO_PUSH_IS_ANGRY PASS\n");return 0; }