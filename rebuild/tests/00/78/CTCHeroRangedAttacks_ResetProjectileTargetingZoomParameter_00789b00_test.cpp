#include <stdio.h>
struct Def{bool Analogue;};struct R{Def* P;float Zoom;void Reset(){Zoom=(P&&P->Analogue)?0.0f:1.0f;}};
int main(){Def d={true};R a={&d,7},b={0,7};a.Reset();b.Reset();if(a.Zoom!=0||b.Zoom!=1)return 1;printf("PROJECTILE_ZOOM_RESET PASS\n");return 0;}