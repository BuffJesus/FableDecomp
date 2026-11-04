// Actual submission, map/tree insertion, CRT nodes, Windows event and overlap lifetime.
#include "fable_threaded_file_object.h"
#include "NativeThreadedReadAPC_win32.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);}
static void Fail(const char* e,int line) {fprintf(stderr,"NATIVE_SUBMISSION_FAILURE %d %s\n",line,e);fflush(stderr);ExitProcess(82);}
#define CHECK(x) do {if(!(x))Fail(#x,__LINE__);}while(0)
typedef CThreadedFileManager::CFileObject Object;
extern "C" void __fastcall SubmissionRemovePending(CThreadedFileManager*,void*,Object*);
#pragma comment(linker,"/alternatename:@SubmissionRemovePending@12=?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z")
static bool g_pendingRemoved[32],g_retired[32];static unsigned g_removals,g_refusals,g_missing;
typedef _STL::map<unsigned long,Object*> UidMap;
typedef _STL::map<CThreadedFileManager::CPendingObjectSortKey,Object*> PendingMap;
struct Node {unsigned char color,pad[3];Node* parent;Node* left;Node* right;unsigned key;Object* value;};
static unsigned char* g_blocks[32];static Object* g_objects[32];static unsigned g_live,g_allocations,g_releases,g_nodeFrees,g_headerFrees,g_submissions;
void* __cdecl operator new(size_t size) {
 CHECK(size==64 && g_live<32);unsigned char* block=static_cast<unsigned char*>(malloc(96));CHECK(block);memset(block,0xcd,96);
 g_blocks[g_live]=block;g_objects[g_live]=reinterpret_cast<Object*>(block+16);++g_allocations;return g_objects[g_live++];
}
void __cdecl operator delete(void* p) {
 unsigned n=0;while(n<g_live && g_objects[n]!=p)++n;CHECK(n<g_live && g_blocks[n]);
 for(unsigned i=0;i<16;++i)CHECK(g_blocks[n][i]==0xcd && g_blocks[n][80+i]==0xcd);
 free(g_blocks[n]);g_blocks[n]=0;++g_releases;
}
static void Put(void* p,unsigned offset,unsigned value) {memcpy(static_cast<unsigned char*>(p)+offset,&value,4);}
static unsigned Get(const void* p,unsigned offset) {unsigned value;memcpy(&value,static_cast<const unsigned char*>(p)+offset,4);return value;}
static bool Less(Object* a,Object* b,bool pending) {
 if(!pending)return a->UID<b->UID;
 if(a->PFile->physicalSortKey_!=b->PFile->physicalSortKey_)return a->PFile->physicalSortKey_<b->PFile->physicalSortKey_;
 if(a->Desc.FilePos!=b->Desc.FilePos)return a->Desc.FilePos<b->Desc.FilePos;
 return reinterpret_cast<unsigned>(a)<reinterpret_cast<unsigned>(b);
}
static Node* g_visited[32];static Node* g_order[32];static unsigned g_seen,g_ordered;
static unsigned Walk(Node* node,Node* parent) {
 if(!node)return 1;CHECK(g_seen<32);for(unsigned i=0;i<g_seen;++i)CHECK(g_visited[i]!=node);g_visited[g_seen++]=node;
 CHECK(node->parent==parent && node->color<=1);
 if(!node->color) {CHECK(!node->left || node->left->color==1);CHECK(!node->right || node->right->color==1);}
 unsigned left=Walk(node->left,node);g_order[g_ordered++]=node;unsigned right=Walk(node->right,node);CHECK(left==right);return left+node->color;
}
static unsigned Validate(void* map,bool pending,unsigned device,unsigned priority) {
 Node* header=reinterpret_cast<Node*>(Get(map,0));CHECK(header && header->color==0);g_seen=g_ordered=0;Walk(header->parent,header);
 Object* expected[32];unsigned count=0;
 for(unsigned n=0;n<g_live;++n)if(!g_retired[n] && (!pending || (!g_pendingRemoved[n] && g_objects[n]->PFile->deviceId_==device && unsigned(g_objects[n]->Desc.Priority)==priority)))expected[count++]=g_objects[n];
 for(unsigned i=1;i<count;++i) {Object* value=expected[i];unsigned j=i;while(j && Less(value,expected[j-1],pending)) {expected[j]=expected[j-1];--j;}expected[j]=value;}
 CHECK(count==g_seen && count==g_ordered && Get(map,4)==count);
 if(count) {CHECK(header->parent->color==1 && header->left==g_order[0] && header->right==g_order[count-1]);}
 else CHECK(header->parent==0 && header->left==header && header->right==header);
 for(unsigned i=0;i<count;++i)CHECK(g_order[i]->value==expected[i] && g_order[i]->key==(pending?reinterpret_cast<unsigned>(expected[i]):expected[i]->UID));
 return count;
}
static void FreeNodes(Node* node) {if(!node)return;FreeNodes(node->left);FreeNodes(node->right);free(node);++g_nodeFrees;}
int main() {
 const unsigned starts[3]={0,0xfffffffe,0xfffffff0},counts[5]={1,2,3,7,15};unsigned cases=0;
 for(unsigned pattern=0;pattern<4;++pattern)for(unsigned start=0;start<3;++start)for(unsigned amount=0;amount<5;++amount) {
  double managerWords[51],fileWords[32];unsigned char* manager=reinterpret_cast<unsigned char*>(managerWords)+16;unsigned char* files=reinterpret_cast<unsigned char*>(fileWords);
  memset(managerWords,0xa5,sizeof(managerWords));memset(fileWords,0xb6,sizeof(fileWords));g_live=0;memset(g_pendingRemoved,0,sizeof(g_pendingRemoved));memset(g_retired,0,sizeof(g_retired));
  for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
   PendingMap* map=new(manager+4+(d*7+p)*16) PendingMap;
   new(manager+4+(d*7+p)*16+12) PendingMap::iterator(map->end());
  }
  UidMap* uid=new(manager+0x13c) UidMap;InitializeCriticalSection(manager+0x14c);HANDLE event=CreateEventA(0,0,0,0);CHECK(event);Put(manager,0x164,reinterpret_cast<unsigned>(event));Put(manager,0x168,starts[start]);
  for(unsigned f=0;f<4;++f) {Put(files+f*64,4,f%2);Put(files+f*64,20,pattern>=2?f/2:f*17);}
  unsigned char expectedManager[sizeof(managerWords)],expectedFiles[sizeof(fileWords)];memcpy(expectedManager,managerWords,sizeof(managerWords));memcpy(expectedFiles,fileWords,sizeof(fileWords));
  for(unsigned n=0;n<counts[amount];++n) {
   CThreadedFileObjectDesc desc;unsigned char* raw=reinterpret_cast<unsigned char*>(&desc);for(unsigned j=0;j<24;++j)raw[j]=static_cast<unsigned char>(j*17+n*31);
   desc.QueueWhenFinished=n%2!=0;desc.FilePos=pattern==0?n*23:pattern==1?(31-n)*23:n%3;desc.Priority=static_cast<EThreadedFilePriority>(1+(pattern==3?0:n%6));
   CThreadedFile* file=reinterpret_cast<CThreadedFile*>(files+(n%4)*64);unsigned wanted=starts[start]+n;
   CHECK(reinterpret_cast<CThreadedFileManager*>(manager)->AddObject(desc,*file)==wanted && g_live==n+1);
   Object* object=g_objects[n];CHECK(object->UID==wanted && object->PFile==file && object->PManager==reinterpret_cast<CThreadedFileManager*>(manager) && object->LoadingStatus==0 && object->Overlapped);
   unsigned char expectedObject[64];memset(expectedObject,0xcd,64);memcpy(expectedObject,&desc,24);Put(expectedObject,0x18,wanted);Put(expectedObject,0x1c,0);Put(expectedObject,0x20,reinterpret_cast<unsigned>(object->Overlapped));Put(expectedObject,0x24,reinterpret_cast<unsigned>(manager));Put(expectedObject,0x28,reinterpret_cast<unsigned>(file));CHECK(memcmp(expectedObject,object,64)==0);
   for(unsigned j=0;j<20;++j)CHECK(reinterpret_cast<unsigned char*>(object->Overlapped)[j]==0);
   CHECK(WaitForSingleObject(event,0)==WAIT_OBJECT_0 && WaitForSingleObject(event,0)==WAIT_TIMEOUT);
   unsigned pendingCount=0;for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
    unsigned count=Validate(manager+4+(d*7+p)*16,true,d,p);pendingCount+=count;Put(expectedManager,16+4+(d*7+p)*16+4,count);
   }
   CHECK(pendingCount==g_live && Validate(uid,false,0,0)==g_live);Put(expectedManager,16+0x140,g_live);Put(expectedManager,16+0x168,wanted+1);
   CHECK(memcmp(expectedManager,managerWords,sizeof(managerWords))==0 && memcmp(expectedFiles,fileWords,sizeof(fileWords))==0);++g_submissions;
  }
  for(unsigned step=0;step<g_live;++step) {
   unsigned n=pattern%2?g_live-1-step:step;Object* object=g_objects[n];
   unsigned device=object->PFile->deviceId_,priority=object->Desc.Priority;
   unsigned char* queue=manager+4+(device*7+priority)*16;
   unsigned count=Validate(queue,true,device,priority),index=0;
   while(index<count && g_order[index]->value!=object)++index;CHECK(index<count);
   Node* header=reinterpret_cast<Node*>(Get(queue,0));Node* target=g_order[index];
   unsigned selection=(step+pattern)%4;
   Node* cursor=selection==0?target:selection==1?header:selection==2?g_order[0]:g_order[count-1];
   Node* expected=cursor==target?(index+1<count?g_order[index+1]:header):cursor;
   Put(queue,12,reinterpret_cast<unsigned>(cursor));
   unsigned char objectCopy[32][64];for(unsigned i=0;i<g_live;++i)if(!g_retired[i])memcpy(objectCopy[i],g_objects[i],64);
   CThreadedFileManager* owner=reinterpret_cast<CThreadedFileManager*>(manager);unsigned uidValue=object->UID;
   unsigned releases=g_releases;
   unsigned char protectedManager[sizeof(managerWords)],protectedObject[64];
   memcpy(protectedManager,managerWords,sizeof(managerWords));
   object->LoadingStatus=CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED;
   memcpy(protectedObject,object,64);
   CHECK(!owner->RemoveObject(uidValue) && g_releases==releases);CHECK(memcmp(protectedManager,managerWords,sizeof(managerWords))==0 && memcmp(protectedObject,object,64)==0);++g_refusals;
   object->LoadingStatus=CThreadedFileManager::FILE_LOADING_STATUS_PENDING;
   if(pattern%2) {
    SubmissionRemovePending(owner,0,object);
    for(unsigned status=2;status<=3;++status) {
     object->LoadingStatus=static_cast<CThreadedFileManager::EFileLoadingStatus>(status);
     memcpy(protectedManager,managerWords,sizeof(managerWords));memcpy(protectedObject,object,64);
     CHECK(!owner->RemoveObject(uidValue) && g_releases==releases);CHECK(memcmp(protectedManager,managerWords,sizeof(managerWords))==0 && memcmp(protectedObject,object,64)==0);++g_refusals;
    }
    object->LoadingStatus=CThreadedFileManager::FILE_LOADING_STATUS_FINISHED;
   }
   CHECK(owner->RemoveObject(uidValue) && g_releases==releases+1);g_retired[n]=true;
   CHECK(Get(uid,4)==g_live-step-1);
   CHECK(owner->RemoveObject(uidValue) && g_releases==releases+1);++g_missing;
   g_pendingRemoved[n]=true;CHECK(Get(queue,12)==reinterpret_cast<unsigned>(expected));
   Put(expectedManager,16+4+(device*7+priority)*16+12,reinterpret_cast<unsigned>(expected));
   unsigned pendingCount=0;for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
    unsigned remaining=Validate(manager+4+(d*7+p)*16,true,d,p);pendingCount+=remaining;
    Put(expectedManager,16+4+(d*7+p)*16+4,remaining);
   }
   CHECK(pendingCount==g_live-step-1 && Validate(uid,false,0,0)==g_live-step-1);Put(expectedManager,16+0x140,g_live-step-1);
   for(unsigned i=0;i<g_live;++i)if(!g_retired[i])CHECK(memcmp(objectCopy[i],g_objects[i],64)==0);
   CHECK(memcmp(expectedManager,managerWords,sizeof(managerWords))==0 && memcmp(expectedFiles,fileWords,sizeof(fileWords))==0);++g_removals;
  }
  for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
   PendingMap* map=reinterpret_cast<PendingMap*>(manager+4+(d*7+p)*16);Node* header=reinterpret_cast<Node*>(Get(map,0));FreeNodes(header->parent);free(header);map->~PendingMap();++g_headerFrees;
  }
  Node* header=reinterpret_cast<Node*>(Get(uid,0));FreeNodes(header->parent);free(header);uid->~UidMap();++g_headerFrees;
  for(unsigned n=0;n<g_live;++n)CHECK(g_retired[n] && !g_blocks[n]);CHECK(g_allocations==g_releases);DeleteCriticalSection(manager+0x14c);CHECK(CloseHandle(event));++cases;
 }
 CHECK(cases==60 && g_submissions==336 && g_allocations==336 && g_releases==336 && g_nodeFrees==0 && g_removals==336 && g_missing==336 && g_refusals==672 && g_headerFrees==900);
 printf("NATIVE_RETIREMENT_PASS cases=60 submissions=336 removals=336 objects=336 refusals=672 missing=336 headers=900\n");return 0;
}
