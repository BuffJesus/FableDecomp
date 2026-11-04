#include "fable_threaded_file_object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int,size_t);
static void Fail(const char* s,int line) {fprintf(stderr,"PENDING_CLOSE_FAILURE %d %s\n",line,s);fflush(stderr);ExitProcess(80);}
#define CHECK(x) do {if(!(x))Fail(#x,__LINE__);}while(0)
typedef CThreadedFileManager::CFileObject Object;
typedef _STL::map<unsigned long,Object*> Map;
struct Node {unsigned color;Node *parent,*left,*right;unsigned key;Object* value;};
static unsigned long* g_manager;static unsigned g_seed,g_mode,g_callbacks;
static Map* Objects() {return reinterpret_cast<Map*>(g_manager+0x13c/4);}
bool CThreadedFileCallback::OnFileReadBegin(void**,EThreadedFilePriority) {Fail("unexpected begin",__LINE__);return false;}
void CThreadedFileCallback::OnFileReadCompletion(bool) {Fail("base callback",__LINE__);}
struct CancelCallback: CThreadedFileCallback {
    void OnFileReadCompletion(bool success) {
        CHECK(!success && ++g_callbacks==1);
        CHECK(g_manager[0x16c/4]==g_seed+1 && !g_manager[(0x14c+8)/4] && !g_manager[(0x14c+12)/4]);
        if(g_mode==2) {
            Map::iterator it=Objects()->find(17);CHECK(it!=Objects()->end());
            Object* object=it->second;Objects()->erase(it);delete object;
        }
    }
};
static Node* NewNode() {Node* n=static_cast<Node*>(malloc(sizeof(Node)));CHECK(n);memset(n,0,sizeof(Node));return n;}
static Object* NewObject(CThreadedFile* file,unsigned uid,unsigned status,unsigned priority,CThreadedFileCallback* callback) {
    Object* o=static_cast<Object*>(malloc(sizeof(Object)));CHECK(o);memset(o,0,sizeof(Object));
    o->PFile=file;o->UID=uid;o->LoadingStatus=static_cast<CThreadedFileManager::EFileLoadingStatus>(status);
    o->Desc.Priority=static_cast<EThreadedFilePriority>(priority);o->Desc.PCallback=callback;
    o->Overlapped=static_cast<_OVERLAPPED*>(GlobalAlloc(0x40,32));CHECK(o->Overlapped);return o;
}
int main() {
    unsigned cases=0;
    for(unsigned seed=0;seed<2;++seed)for(unsigned mode=0;mode<3;++mode)for(unsigned same=0;same<2;++same)for(unsigned priority=1;priority<=6;priority+=5) {
        unsigned long storage[(0x178+32)/4];memset(storage,0xcd,sizeof(storage));g_manager=storage+4;memset(g_manager,0,0x178);
        g_seed=seed?7:0;g_mode=mode;g_callbacks=0;g_manager[0x16c/4]=g_seed;
        InitializeCriticalSection(g_manager+0x14c/4);
        unsigned long fileWords[7]={0},otherWords[7]={0};
        CThreadedFile* file=reinterpret_cast<CThreadedFile*>(fileWords);CThreadedFile* other=reinterpret_cast<CThreadedFile*>(otherWords);
        CancelCallback callback;
        Object* pending=NewObject(file,17,0,priority,mode?&callback:0);
        Object* finished=NewObject(same?file:other,20,4,3,0);
        Node* head=NewNode();Node* first=NewNode();Node* last=NewNode();
        first->parent=last;first->key=17;first->value=pending;
        last->color=1;last->parent=head;last->left=first;last->key=20;last->value=finished;
        head->parent=last;head->left=first;head->right=last;
        g_manager[0x13c/4]=reinterpret_cast<unsigned long>(head);g_manager[0x140/4]=2;
        Node* pendingHead=NewNode();Node* pendingNode=NewNode();pendingNode->color=1;pendingNode->parent=pendingHead;
        pendingNode->key=reinterpret_cast<unsigned long>(pending);pendingNode->value=pending;
        pendingHead->parent=pendingNode;pendingHead->left=pendingNode;pendingHead->right=pendingNode;
        unsigned long* queue=g_manager+(4+16*priority)/4;
        queue[0]=reinterpret_cast<unsigned long>(pendingHead);queue[1]=1;queue[3]=reinterpret_cast<unsigned long>(pendingNode);
        reinterpret_cast<CThreadedFileManager*>(g_manager)->CloseAllStreamsForFile(file);
        CHECK(g_callbacks==unsigned(mode!=0) && g_manager[0x16c/4]==g_seed);
        CHECK(!queue[1] && queue[3]==reinterpret_cast<unsigned long>(pendingHead));
        CHECK(!pendingHead->parent && pendingHead->left==pendingHead && pendingHead->right==pendingHead);
        CHECK(g_manager[0x140/4]==unsigned(!same) && Objects()->find(17)==Objects()->end());
        if(!same) {
            Map::iterator it=Objects()->find(20);CHECK(it!=Objects()->end() && it->second==finished);
            Objects()->erase(it);delete finished;
        }
        CHECK(!g_manager[0x140/4] && !head->parent && head->left==head && head->right==head);
        for(unsigned i=0;i<4;++i)CHECK(storage[i]==0xcdcdcdcd && storage[4+0x178/4+i]==0xcdcdcdcd);
        DeleteCriticalSection(g_manager+0x14c/4);free(head);free(pendingHead);++cases;
    }
    printf("PENDING_CLOSE_HOST_PASS cases=%u\n",cases);return 0;
}
