#include <stdio.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long __fastcall CountPixelReferences(void*);
#pragma comment(linker,"/alternatename:@CountPixelReferences@4=?CountReferences@CPixelShader@@EAEKXZ")
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
 void* arena=VirtualAlloc((void*)0x24000000,0x1000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned cases=0;while(fread(arena,1,64,input)==64){unsigned result=CountPixelReferences(arena);fwrite(&result,4,1,output);fwrite(arena,1,64,output);++cases;}
 fclose(input);fclose(output);printf("NATIVE_PIXEL_REFERENCES_PASS cases=%u\n",cases);return 0;
}
