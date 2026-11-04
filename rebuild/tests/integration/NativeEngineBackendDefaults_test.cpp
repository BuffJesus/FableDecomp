#include <stdio.h>
#include <stdlib.h>
#include "engine_backend_defaults_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char __fastcall Default11(void*,void*,unsigned,unsigned);
extern "C" unsigned char __fastcall Default15(void*,void*);
extern "C" unsigned char __fastcall Default16(void*,void*,unsigned,unsigned,unsigned);
extern "C" unsigned __fastcall Default37(void*,void*);
extern "C" void __fastcall Default38(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" unsigned char __fastcall Default39(void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default40(void*,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default48(void*,void*,unsigned);
extern "C" void __fastcall Default49(void*,void*);
extern "C" void __fastcall Default53(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Default54(void*,void*);
extern "C" void __fastcall Default64(void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default65(void*,void*,unsigned,unsigned,unsigned);
extern "C" void __fastcall Default68(void*,void*,unsigned);
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
unsigned p[7],cases=0;while(fread(p,4,7,input)==7){if(fread(arena,1,1024,input)!=1024)return 5;unsigned value=0;switch(p[0]){
case 11:{value=(unsigned)Default11(arena,0,p[1],p[2]); break;}
case 15:{value=(unsigned)Default15(arena,0); break;}
case 16:{value=(unsigned)Default16(arena,0,p[1],p[2],p[3]); break;}
case 37:{value=(unsigned)Default37(arena,0); break;}
case 38:{Default38(arena,0,p[1],p[2],p[3],p[4]); break;}
case 39:{value=(unsigned)Default39(arena,0,p[1],p[2],p[3]); break;}
case 40:{Default40(arena,0,p[1],p[2],p[3],p[4]); break;}
case 48:{Default48(arena,0,p[1]); break;}
case 49:{Default49(arena,0); break;}
case 53:{Default53(arena,0,p[1],p[2]); break;}
case 54:{Default54(arena,0); break;}
case 64:{Default64(arena,0,p[1],p[2],p[3]); break;}
case 65:{Default65(arena,0,p[1],p[2],p[3]); break;}
case 68:{Default68(arena,0,p[1]); break;}
default:return 6;}unsigned h[]={p[0],value};fwrite(h,4,2,output);fwrite(arena,1,1024,output);++cases;}
fclose(input);fclose(output);printf("NATIVE_ENGINE_BACKEND_DEFAULTS_PASS cases=%u\n",cases);return 0;}
