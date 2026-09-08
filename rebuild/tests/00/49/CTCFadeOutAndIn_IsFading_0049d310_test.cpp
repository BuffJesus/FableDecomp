#include <stdio.h>
struct Fade { void* v; unsigned char base[8]; bool FadingOut; bool FadingIn;
 unsigned char align[2]; float Alpha; long Frames; unsigned char Things[16]; bool IsFading() const; };
bool Fade::IsFading() const{return FadingIn||FadingOut;}
int main(){Fade fade;fade.FadingIn=false;fade.FadingOut=false;if(fade.IsFading())return 1;
 fade.FadingIn=true;if(!fade.IsFading())return 2;fade.FadingIn=false;fade.FadingOut=true;
 if(!fade.IsFading())return 3;printf("FADE_OUT_AND_IN_IS_FADING PASS\n");return 0;}