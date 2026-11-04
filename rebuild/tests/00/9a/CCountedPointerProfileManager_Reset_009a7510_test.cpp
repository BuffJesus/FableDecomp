#include <stdio.h>
#include <stdlib.h>
#include "fable_system_manager.h"
static bool failInfo;static int destroyed;
void* __cdecl operator new(size_t n){if(failInfo && n==sizeof(CCPPointerInfo))return 0;return malloc(n);}
void __cdecl operator delete(void* p){free(p);}
namespace _STL {
template<> list<CProfile*>::~list(){free(_M_node);}
template<> list<CProfileManager::CProfileTreeEntry>::~list(){free(_M_node);}
}
CProfileManager::CProfileManager() {}
CProfileManager::~CProfileManager(){++destroyed;free(IDToProfileName._M_t._M_header._M_data);free(ProfileNameToID._M_t._M_header._M_data);}
int main(){
 CCountedPointer<CProfileManager> p;p.Reset(0);if(p.Data||p.Info||destroyed)return 1;
 CProfileManager* a=new CProfileManager;p.Reset(a);if(p.Data!=a||!p.Info||p.Info->Data!=a||p.Info->RefCount!=1||!p.Info->DeleteFunc)return 2;
 CCPPointerInfo* old=p.Info;old->RefCount=2;CProfileManager* b=new CProfileManager;p.Reset(b);
 if(destroyed||old->RefCount!=1||old->Data!=a||p.Data!=b||p.Info==old||p.Info->Data!=b||p.Info->RefCount!=1)return 3;
 p.Reset(0);if(destroyed!=1||p.Data||p.Info)return 4;
 old->DeleteFunc(old->Data);delete old;if(destroyed!=2)return 5;
 CProfileManager* c=new CProfileManager;failInfo=true;p.Reset(c);failInfo=false;
 if(p.Data!=c||p.Info||destroyed!=2)return 6;
 p.Reset(0);delete c;if(destroyed!=3||p.Data||p.Info)return 7;
 puts("PROFILE_RESET_PASS");return 0;
}