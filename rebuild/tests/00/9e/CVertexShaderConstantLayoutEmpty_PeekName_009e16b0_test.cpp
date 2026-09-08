#include <stdio.h>
#include <string.h>
struct CCharString { const char* Data; CCharString(const char* text,int):Data(text){} };
char kVertexShaderConstantLayoutEmptyName[]="engineEmptyVSConstantLayout";
struct Layout { CCharString PeekName() const; };
CCharString Layout::PeekName() const{return CCharString(kVertexShaderConstantLayoutEmptyName,-1);}
int main(){Layout x;if(strcmp(x.PeekName().Data,kVertexShaderConstantLayoutEmptyName))return 1;
 printf("VERTEX_LAYOUT_EMPTY_NAME PASS\n");return 0;}