#include <stdio.h>
static long g_values[4];
struct CLandscapeBackgroundMap { bool result; bool IsAtFullDetailInArea(long,long,long,long); };
bool CLandscapeBackgroundMap::IsAtFullDetailInArea(long a,long b,long c,long d) {
 g_values[0]=a; g_values[1]=b; g_values[2]=c; g_values[3]=d; return result; }
struct CEngineLandscapeMap { void* v; CLandscapeBackgroundMap* background;
 bool IsBackgroundAtFullDetailInArea(long,long,long,long); };
bool CEngineLandscapeMap::IsBackgroundAtFullDetailInArea(long a,long b,long c,long d) {
 if(background==0) return true; return background->IsAtFullDetailInArea(a,b,c,d); }
int main() { CEngineLandscapeMap map; map.background=0;
 if(!map.IsBackgroundAtFullDetailInArea(1,2,3,4)) return 1;
 CLandscapeBackgroundMap background; background.result=false; map.background=&background;
 if(map.IsBackgroundAtFullDetailInArea(5,6,7,8)) return 2;
 if(g_values[0]!=5 || g_values[3]!=8) return 3;
 printf("BACKGROUND_FULL_DETAIL PASS\n"); return 0; }