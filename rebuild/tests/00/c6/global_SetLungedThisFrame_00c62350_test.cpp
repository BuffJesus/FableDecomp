#include <cstdio>
static int gVal = 7;
static int getCountModel(){ return gVal; }
static float scaleModel = 0.5f;
static float model(){ return (float)getCountModel() * scaleModel; }
int main(){
    gVal = 7; scaleModel = 0.5f;
    if (model() != 3.5f) { printf("FAIL\n"); return 1; }
    gVal = 0;
    if (model() != 0.0f) { printf("FAIL\n"); return 1; }
    gVal = -4; scaleModel = 2.0f;
    if (model() != -8.0f) { printf("FAIL\n"); return 1; }
    printf("OK_00c62350\n");
    return 0;
}