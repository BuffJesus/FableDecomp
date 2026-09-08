#include <stdio.h>
struct Colour{float R,G,B,A;};struct Engine{Colour PeekGlobalAmbientColour()const;};
Colour Engine::PeekGlobalAmbientColour()const{Colour value={1,1,1,1};return value;}
int main(){Engine engine;Colour value=engine.PeekGlobalAmbientColour();
 if(value.R!=1 || value.G!=1 || value.B!=1 || value.A!=1)return 1;
 printf("IENGINE_GLOBAL_AMBIENT_COLOUR PASS\n");return 0;}