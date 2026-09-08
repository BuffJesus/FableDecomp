#include <stdio.h>
struct Fade{bool Active;unsigned char pad[11];bool Closing;bool Opening;bool IsOpen()const;};
bool Fade::IsOpen()const{return Active&&!Opening&&!Closing;}
int main(){Fade f;f.Active=true;f.Opening=false;f.Closing=false;if(!f.IsOpen())return 1;
 f.Opening=true;if(f.IsOpen())return 2;f.Opening=false;f.Closing=true;if(f.IsOpen())return 3;
 f.Closing=false;f.Active=false;if(f.IsOpen())return 4;printf("FADE_IS_OPEN PASS\n");return 0;}