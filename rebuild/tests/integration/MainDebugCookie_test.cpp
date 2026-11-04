#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" unsigned long __security_cookie;
extern "C" __declspec(dllimport) unsigned long __stdcall SetErrorMode(unsigned long);
class Access:public CMemoryAllocator{public:using CMemoryAllocator::InternalAllocate;};
static unsigned char owner[0x49c],data[256];static unsigned long mode,queries,calls;
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool debug){if(size!=1||category!=9||!debug)exit(7);++calls;if(mode)__security_cookie^=0x12345678;return data;}
unsigned long CMemoryAllocator::GetAllocationSize(void* p){if(p!=data)exit(8);++queries;if(queries==2){fprintf(stderr,"COOKIE_BOUNDARY_REACHED\n");fflush(stderr);}return 256;}
static void __cdecl failure(int code,void*){if(code!=_SECERR_BUFFER_OVERRUN)_exit(9);fprintf(stderr,"CRT_COOKIE_MISMATCH_DETECTED\n");fflush(stderr);_exit(3);}
int main(int argc,char** argv){if(argc!=3)return 2;_set_error_mode(_OUT_TO_STDERR);_set_security_error_handler(failure);SetErrorMode(3);mode=strtoul(argv[1],0,0);__security_cookie=strtoul(argv[2],0,0);memset(owner,0,sizeof(owner));owner[0x414]=255;memset(data,0xa5,sizeof(data));void* result=((Access*)owner)->InternalAllocate(1,9,(const char*)0xdeadbeef,-1,(const char*)0xbaadf00d);if(result!=data||calls!=1||queries!=2)return 4;unsigned long* trailer=(unsigned long*)(data+236);if(trailer[0]!=0xffffffff||trailer[1]!=255||trailer[2]!=1)return 5;printf("MAIN_DEBUG_COOKIE_RETURN_OK\n");return 0;}
