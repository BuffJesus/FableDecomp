#include <stdio.h>
#include <stdlib.h>
#include "fable_system_manager.h"
static bool failInfo;static int destroyed;
void* __cdecl operator new(size_t n){if(failInfo && n==sizeof(CCPPointerInfo))return 0;return malloc(n);}
void __cdecl operator delete(void* p){free(p);}
CDriveManager::~CDriveManager(){++destroyed;}
int main(){
 CCountedPointer<CDriveManager> p;p.Reset(0);if(p.Data||p.Info||destroyed)return 1;
 CDriveManager* a=new CDriveManager;p.Reset(a);if(p.Data!=a||!p.Info||p.Info->Data!=a||p.Info->RefCount!=1||!p.Info->DeleteFunc)return 2;
 CCPPointerInfo* old=p.Info;old->RefCount=2;CDriveManager* b=new CDriveManager;p.Reset(b);
 if(destroyed||old->RefCount!=1||old->Data!=a||p.Data!=b||p.Info==old||p.Info->Data!=b||p.Info->RefCount!=1)return 3;
 p.Reset(0);if(destroyed!=1||p.Data||p.Info)return 4;
 old->DeleteFunc(old->Data);delete old;if(destroyed!=2)return 5;
 CDriveManager* c=new CDriveManager;failInfo=true;p.Reset(c);failInfo=false;
 if(p.Data!=c||p.Info||destroyed!=2)return 6;
 p.Reset(0);delete c;if(destroyed!=3||p.Data||p.Info)return 7;
 puts("DRIVE_RESET_PASS");return 0;
}