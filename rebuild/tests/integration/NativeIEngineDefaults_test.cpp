#include <stdio.h>
#include <stdlib.h>
#include "iengine_defaults_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char __fastcall Default14(void*,void*,unsigned);
extern "C" unsigned char __fastcall Default18(void*,void*,unsigned,unsigned);
extern "C" float __fastcall Default19(void*,void*);
extern "C" unsigned char __fastcall Default21(void*,void*,unsigned);
extern "C" unsigned char __fastcall Default22(void*,void*,unsigned);
extern "C" unsigned __fastcall Default26(void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default33(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default34(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" unsigned __fastcall Default35(void*,void*,unsigned);
extern "C" unsigned __fastcall Default36(void*,void*,unsigned,unsigned);
extern "C" unsigned char __fastcall Default39(void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default49(void*,void*);
extern "C" void __fastcall Default50(void*,void*);
extern "C" unsigned char __fastcall Default51(void*,void*);
extern "C" void __fastcall Default53(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Default60(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" unsigned __fastcall Default69(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" unsigned char __fastcall Default74(void*,void*);
extern "C" void* __fastcall Default83(void*,void*,unsigned);
extern "C" void* __fastcall Default84(void*,void*,unsigned);
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
unsigned p[7],cases=0;while(fread(p,4,7,input)==7){if(fread(arena,1,1024,input)!=1024)return 5;unsigned value=0;switch(p[0]){
case 14:{value=(unsigned)Default14(arena,0,p[1]); break;}
case 18:{value=(unsigned)Default18(arena,0,p[1],p[2]); break;}
case 19:{union{float f;unsigned u;}r;r.f=Default19(arena,0);value=r.u; break;}
case 21:{value=(unsigned)Default21(arena,0,p[1]); break;}
case 22:{value=(unsigned)Default22(arena,0,p[1]); break;}
case 26:{value=(unsigned)Default26(arena,0,p[1],p[2],p[3]); break;}
case 33:{Default33(arena,0,p[1],p[2],p[3],p[4]); break;}
case 34:{Default34(arena,0,p[1],p[2],p[3],p[4]); break;}
case 35:{value=(unsigned)Default35(arena,0,p[1]); break;}
case 36:{value=(unsigned)Default36(arena,0,p[1],p[2]); break;}
case 39:{value=(unsigned)Default39(arena,0,p[1],p[2],p[3]); break;}
case 49:{Default49(arena,0); break;}
case 50:{Default50(arena,0); break;}
case 51:{value=(unsigned)Default51(arena,0); break;}
case 53:{Default53(arena,0,p[1],p[2]); break;}
case 60:{Default60(arena,0,p[1],p[2],p[3],p[4]); break;}
case 69:{value=(unsigned)Default69(arena,0,p[1],p[2],p[3],p[4]); break;}
case 74:{value=(unsigned)Default74(arena,0); break;}
case 83:{value=(unsigned)Default83(arena,0,p[1]); break;}
case 84:{value=(unsigned)Default84(arena,0,p[1]); break;}
default:return 6;}unsigned h[]={p[0],value};fwrite(h,4,2,output);fwrite(arena,1,1024,output);++cases;}
fclose(input);fclose(output);printf("NATIVE_IENGINE_DEFAULTS_PASS cases=%u\n",cases);return 0;}
