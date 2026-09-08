#include <stdio.h>
struct Display{long CurrentAlpha;bool HasArea;bool IsDrawing()const{return CurrentAlpha>0&&HasArea;}};
int main(){Display a={0,true},b={1,false},c={1,true};if(a.IsDrawing()||b.IsDrawing()||!c.IsDrawing())return 1;printf("REGION_DISPLAY_DRAWING PASS\n");return 0;}