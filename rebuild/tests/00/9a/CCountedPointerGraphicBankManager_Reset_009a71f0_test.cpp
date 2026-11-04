#include <stdio.h>
#include <stdlib.h>
#include "fable_system_manager.h"
static bool failInfo;static int destroyed;
void* __cdecl operator new(size_t n){if(failInfo && n==sizeof(CCPPointerInfo))return 0;return malloc(n);}
void __cdecl operator delete(void* p){free(p);}
#include "test_support/base_class_stubs.h"
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
CGraphicBankManager::CGraphicBankManager(){} CGraphicBankManager::~CGraphicBankManager(){++destroyed;}

int main(){
 CCountedPointer<CGraphicBankManager> p;p.Reset(0);if(p.Data||p.Info||destroyed)return 1;
 CGraphicBankManager* a=new CGraphicBankManager;p.Reset(a);if(p.Data!=a||!p.Info||p.Info->Data!=a||p.Info->RefCount!=1||!p.Info->DeleteFunc)return 2;
 CCPPointerInfo* old=p.Info;old->RefCount=2;CGraphicBankManager* b=new CGraphicBankManager;p.Reset(b);
 if(destroyed||old->RefCount!=1||old->Data!=a||p.Data!=b||p.Info==old||p.Info->Data!=b||p.Info->RefCount!=1)return 3;
 p.Reset(0);if(destroyed!=1||p.Data||p.Info)return 4;
 old->DeleteFunc(old->Data);delete old;if(destroyed!=2)return 5;
 CGraphicBankManager* c=new CGraphicBankManager;failInfo=true;p.Reset(c);failInfo=false;
 if(p.Data!=c||p.Info||destroyed!=2)return 6;
 p.Reset(0);delete c;if(destroyed!=3||p.Data||p.Info)return 7;
 puts("BANK_RESET_PASS");return 0;
}