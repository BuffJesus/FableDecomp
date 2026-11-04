#include "test_support/bank_extended_virtual_boundaries.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CBankFileAsync* bank;
static void* heapBank;
static void* asyncVtable;
static bool held, deletedSection;
static unsigned errors, abortCalls, sleeps, erased;
static char events[80]; static unsigned count;
static void record(char c) { events[count++]=c; events[count]=0; }
static void require(bool value) { if(!value) ++errors; }
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::~CCharString() {}
CWideString::~CWideString() {}
CPackedUIntArray::~CPackedUIntArray() {}
CChecksumCacheFile::~CChecksumCacheFile() {}
typedef _STL::map<CCharString,unsigned long>::_Rep_type StringTree;
typedef _STL::map<CCharString,unsigned long>::value_type StringValue;
template<> void StringTree::_M_erase(_STL::_Rb_tree_node<StringValue>*) { ++errors; }
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::_Rep_type CacheTree;
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::value_type CacheValue;
template<> void CacheTree::_M_erase(_STL::_Rb_tree_node<CacheValue>*) { ++errors; }
CBankFile::~CBankFile() { require(this==bank && !held && deletedSection); record('B'); }
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) {return false;}
void CBankFile::OpenReadWrite(const CCharString&,unsigned long,unsigned long) {}
bool CBankFile::OpenFileReadOnly(const CWideString&,unsigned long) {return false;}
void CBankFile::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) {}
void CBankFile::Close() {}
bool CBankFileAsync::OpenReadOnly(const CCharString&,unsigned long) {return false;}
void CBankFileAsync::OpenReadWrite(const CCharString&,unsigned long,unsigned long) {}
bool CBankFileAsync::OpenFileReadOnly(const CWideString&,unsigned long) {return false;}
void CBankFileAsync::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long) {}
void CBankFileAsync::Close() {}
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section,bool tryOnly) : CriticalSection(&section),Locked(true) {
    require(!tryOnly && &section==reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank->CriticalSection) && !held);
    asyncVtable=*reinterpret_cast<void**>(bank);
    held=true;record('L');
}
void CCriticalSectionLock::Release() { require(held && Locked);held=false;Locked=false;record('R'); }
void CCriticalSectionLock::Lock() { require(!held && !Locked && !deletedSection);held=true;Locked=true;record('L'); }
CCriticalSectionLock::~CCriticalSectionLock() { require(!held && !Locked && deletedSection);record('G'); }
static void __stdcall fakeSleep(unsigned long ms) { require(ms==10 && !held && !deletedSection);++sleeps;record('S'); }
static void __stdcall fakeDelete(_RTL_CRITICAL_SECTION* cs) { require(cs==reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank->CriticalSection) && !held);deletedSection=true;record('D'); }
static CBankFileAsyncData* dataA=reinterpret_cast<CBankFileAsyncData*>(0x1000);
static CBankFileAsyncData* dataB=reinterpret_cast<CBankFileAsyncData*>(0x2000);
bool CBankFileAsyncData::Abort() {
    require(held && !deletedSection);
    ++abortCalls;
    require(this==(abortCalls<=3?dataA:dataB)); record('A');
    return abortCalls>2;
}
typedef _STL::_Rb_tree_node<CBankFileAsyncData*> AsyncNode;
typedef _STL::set<CBankFileAsyncData*>::_Rep_type AsyncTree;
_STL::_Rb_tree_node_base* __cdecl FableBankEraseTreeNode(_STL::_Rb_tree_node_base* node,_STL::_Rb_tree_node_base*& root,_STL::_Rb_tree_node_base*& left,_STL::_Rb_tree_node_base*& right) {
    AsyncNode* h=bank->AsyncDataInstances._M_t._M_header._M_data;
    require(held && node==left && &root==&h->_M_parent && &left==&h->_M_left && &right==&h->_M_right);
    root=node->_M_right;left=root?root:h;if(!root)right=h;
    ++erased;record('E');return node;
}
template<> void AsyncTree::_M_erase(AsyncNode* node) { require(this==&bank->ReadsToRestart._M_t && !held && deletedSection);free(node);record('T'); }
template<> void _STL::list<CCountedPointer<CBankFileAsyncData> >::clear() { require(this==&bank->DisposalList && deletedSection && !held);record('C'); }
void CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync* value) { require(value==bank && this==bank->MemoryFailureHandler.Data && !held && !deletedSection);record('U'); }
static CCPPointerInfo* owners[3];
static void __fastcall releaseData(void* value) {
    require(!held && deletedSection);
    for(unsigned i=0;i<3;++i)if(value==reinterpret_cast<void*>(0x3000+i*0x100)) {record("hmt"[i]);return;}
    ++errors;
}
void operator delete(void* p) {
    if(p==heapBank) {record('Z');free(p);return;}
    for(unsigned i=0;i<3;++i)if(p==owners[i]) {record("123"[i]);free(p);return;}
    free(p);
}
static void setupTree(FableBankOwnedSet<CBankFileAsyncData*>& set,unsigned n) {
    AsyncNode* h=static_cast<AsyncNode*>(malloc(sizeof(AsyncNode)));memset(h,0,sizeof(*h));
    h->_M_left=h->_M_right=h; set._M_t._M_header._M_data=h;set._M_t._M_node_count=n;
    AsyncNode* prev=0;
    for(unsigned i=0;i<n;++i) {
        AsyncNode* node=static_cast<AsyncNode*>(malloc(sizeof(AsyncNode)));memset(node,0,sizeof(*node));node->_M_value_field=i?dataB:dataA;
        if(prev)prev->_M_right=node;else h->_M_parent=h->_M_left=node;
        h->_M_right=node;prev=node;
    }
}
static void setup(bool populated) {
    count=errors=abortCalls=sleeps=erased=0;held=deletedSection=false;memset(events,0,sizeof(events));
    memset(bank,0,sizeof(*bank));
    setupTree(bank->AsyncDataInstances,populated?2:0);setupTree(bank->ReadsToRestart,populated?1:0);
    bank->DisposalList._M_node=static_cast<_STL::list<CCountedPointer<CBankFileAsyncData> >::_Node*>(malloc(sizeof(_STL::list<CCountedPointer<CBankFileAsyncData> >::_Node)));
}
static int RunCases() {
    unsigned char storage[sizeof(CBankFileAsync)];bank=reinterpret_cast<CBankFileAsync*>(storage);
    setup(false); bank->CBankFileAsync::~CBankFileAsync();
    if(errors || strcmp(events,"LRDGCB") || abortCalls || sleeps || erased)return 1;
    setup(true);
    for(unsigned i=0;i<3;++i)owners[i]=new CCPPointerInfo(releaseData,reinterpret_cast<void*>(0x3000+i*0x100));
    bank->MemoryFailureHandler.Data=reinterpret_cast<CLoadingMemoryFailureHandler*>(0x3000);bank->MemoryFailureHandler.Info=owners[0];
    bank->LoadingMemoryPool.Data=reinterpret_cast<CMovableResourceMemoryPool*>(0x3100);bank->LoadingMemoryPool.Info=owners[1];owners[1]->RefCount=2;
    bank->ThreadedFile.Data=reinterpret_cast<CThreadedFile*>(0x3200);bank->ThreadedFile.Info=owners[2];
    bank->CBankFileAsync::~CBankFileAsync();
    bool ok=errors==0 && strcmp(events,"LARSLARSLAEAERUDGCTh1t3B")==0 && abortCalls==4 && sleeps==2 && erased==2;
    ok=ok && owners[1]->RefCount==1 && !bank->ThreadedFile.Data && !bank->ThreadedFile.Info && !bank->LoadingMemoryPool.Data && !bank->LoadingMemoryPool.Info && !bank->MemoryFailureHandler.Data && !bank->MemoryFailureHandler.Info;
    free(owners[1]);
    if(!ok)return 2;
    heapBank=malloc(sizeof(CBankFileAsync));bank=static_cast<CBankFileAsync*>(heapBank);
    setup(false);*reinterpret_cast<void**>(bank)=asyncVtable;
    delete bank;
    return errors || strcmp(events,"LRDGCBZ") ? 3 : 0;
}
extern "C" __declspec(dllimport) int __stdcall VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" char __ImageBase;
    void** FindImportSlot(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        const fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        if (rva == 0)
            return 0;
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                {
                    return &slots[i];
                }
            }
        }
        return 0;
    }

    bool SwapImport(const char* name, void* replacement, void** previous)
    {
        void** slot = FindImportSlot(name);
        unsigned long oldProtect = 0;
        if (slot == 0 ||
            !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
        {
            return false;
        }
        if (previous != 0)
            *previous = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }


int main() {
    void *sleep=0,*del=0;
    if(!SwapImport("Sleep",(void*)&fakeSleep,&sleep) || !SwapImport("DeleteCriticalSection",(void*)&fakeDelete,&del))return 90;
    int result=RunCases();
    if(!SwapImport("Sleep",sleep,0) || !SwapImport("DeleteCriticalSection",del,0))return 91;
    if(!result)puts("ASYNC_BANK_DTOR_009d6490_PASS");
    else printf("failure %d errors %u events %s\n",result,errors,events);
    return result;
}
