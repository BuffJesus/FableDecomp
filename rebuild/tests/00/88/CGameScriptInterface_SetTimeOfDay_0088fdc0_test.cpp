#include <cstdio>
static float observed; static int calls;
static void SetBarPercentage(float v){observed=v;++calls;}
static void SetTime(float v,float wrap,float scale){for(;wrap<=v;v-=wrap){}SetBarPercentage(v*scale);}
static bool Near(float a,float b){float d=a-b;return d<0?-d<0.0001f:d<0.0001f;}
int main(){SetTime(25.0f,24.0f,1.0f/24.0f);if(calls!=1||!Near(observed,1.0f/24.0f))return 1;SetTime(72.0f,24.0f,1.0f/24.0f);if(calls!=2||!Near(observed,0.0f))return 2;SetTime(-3.0f,24.0f,1.0f/24.0f);if(calls!=3||!Near(observed,-0.125f))return 3;std::printf("SET_TIME_OF_DAY_PASS\n");return 0;}